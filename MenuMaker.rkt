#lang racket

(require racket/match)
(require racket/struct)
(require xml)
(require db)

(define ENTRY-PATTERN "^\\[(.*)\\]$")
(define SETTING-PATTERN "^(.*)\\=(.*)$")
(define EMPTY-LINE "^[\\s]*$")
(define COMMENT "^#.*$")
(define QUOTE "\"")
(define COMMA ",")
(define CONSOLE "konsole -e ")

(struct Setting
  (entry key-value-pairs)
  #:extra-name SETTING
  #:methods gen:custom-write
  [(define write-proc
     (make-constructor-style-printer
      (lambda (obj) 'Setting)
      (lambda (obj) (list (Setting-entry obj)
                          (Setting-key-value-pairs obj)))))])

(struct Application
  (path setting-list)
  #:extra-name APPLICATION
  #:methods gen:custom-write
  [(define write-proc
     (make-constructor-style-printer
      (lambda (obj) 'Application)
      (lambda (obj) (list (Application-path obj)
                          (Application-setting-list obj)))))])

(define (applications-path directory-path)
  (find-files (lambda (path)
                (not (equal? (path->string path) directory-path)))
              directory-path))


(define (parse-settings entry lines)
  (define (iter lines result)
    (if (null? lines)
        (list lines (Setting entry result))
        (match (car lines)
          [(regexp SETTING-PATTERN (list _ key value))
           (iter (cdr lines)
                 (if (null? result)
                     (cons (list key value) '())
                     (cons (list key value) result)))]
          [(regexp EMPTY-LINE) (iter (cdr lines) result)]
          [(regexp COMMENT) (iter (cdr lines) result)]
          [_ (list lines (Setting entry result))])))
  (iter lines '()))

(define (parse-desktop-file path)
  (define text (file->string path))
  (define lines (string-split text "\n"))
  (define (iter lines setting-list)
    (if (null? lines)
        setting-list
        (match (car lines)
          [(regexp ENTRY-PATTERN (list _ entry))
           (match (parse-settings entry (cdr lines))
             [(list lines setting) (iter lines
                                         (if (null? setting-list)
                                             (cons setting '())
                                             (cons setting setting-list)))])]
          [(regexp EMPTY-LINE) (iter (cdr lines) setting-list)]
          [(regexp COMMENT) (iter (cdr lines) setting-list)]
          [a (begin (displayln a) (error "wrong format"))])))
  (iter lines '()))

(define application-path-list
  (applications-path "/usr/share/applications"))

(define application-list
  (map (lambda (path)
         (Application path (parse-desktop-file path)))
       application-path-list))

(define (apps->info application-list)
  (filter (lambda (item) (not (equal? item '())))
          (map (lambda (app)
                 (define setting-list (Application-setting-list app))
                 (define desktop-list
                   (filter (lambda (setting)
                             (equal? (Setting-entry setting) "Desktop Entry"))
                           setting-list))
                 (map (lambda (setting)
                        (filter
                         (lambda (pair)
                           (match pair
                             [(list key value) (or (equal? key "Name")
                                                   (equal? key "Terminal")
                                                   (equal? key "Icon")
                                                   (equal? key "Exec")
                                                   (equal? key "Type"))]
                             [_ #f]
                             ))
                         (Setting-key-value-pairs setting)))
                      desktop-list))
               application-list)))

(define apps-info (apps->info application-list))

(define (process-apps-info apps-info)
  (filter (lambda (item) (not (equal? item #f)))
          (map (lambda (app)
                 (define table (make-hash (car app)))
                 (define (key-value-pair key)
                   (list key (car (hash-ref table key))))
                 (if
                  (and (hash-has-key? table "Name")
                       (hash-has-key? table "Terminal")
                       (hash-has-key? table "Type")
                       (hash-has-key? table "Exec")
                       (hash-has-key? table "Icon"))
                  (list
                   (key-value-pair "Name")
                   (key-value-pair "Terminal")
                   (key-value-pair "Type")
                   (key-value-pair "Exec")
                   (key-value-pair "Icon"))
                  #f))
               apps-info)))

(define (drop-command-code command)
  (define (remove-code text to-remove)
    (if (null? to-remove)
        text
        (remove-code
         (string-replace text (car to-remove) "")
         (cdr to-remove))))
  (remove-code
   command
   (list "%f" "%F" "%u" "%U" "%i" "%c" "%k" "%d" "%D" "%n" "%N" "%v" "%m")))

(define (terminal-app-process terminal command)
  (if (equal? terminal "true")
      (string-append* (list CONSOLE command))
      command))

(define (processed-apps-info->xml processed-apps-info)
  (define app-types (group-by (lambda (pair) (car pair))
                              (map
                               (lambda (app)
                                 (match app
                                   [(list (list "Name" name)
                                          (list "Terminal" terminal)
                                          (list "Type" type)
                                          (list "Exec" exec)
                                          (list "Icon" icon))
                                    (list type app)]))
                               processed-apps-info)))
  (define type-list
    (map
     (lambda (item) (caar item))
     app-types))

  (string-append*
   (list
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<openbox_menu xmlns=\"http://openbox.org/3.4/menu\">"
    (string-append*
     (map
      (lambda (app-type)
        (define app-list
          (map (lambda (item)
                 (match item
                   [(list type items) items]))
               app-type))
        (define type
          (caar app-type))
        (string-append*
         (list
          (string-append* (list
                           "<menu id=\"" type
                           "\" label=\"" type "\">"))
        
          (string-append*
           (map
            (lambda (app)
              (match app
                [(list (list "Name" name)
                       (list "Terminal" terminal)
                       (list "Type" type)
                       (list "Exec" exec)
                       (list "Icon" icon))
                 (string-append*
                  (list
                   (string-append* (list "<item label=\"" name "\">"))
                   "<action name=\"Execute\">"
                   (string-append* (list "<command>" (terminal-app-process
                                                      terminal
                                                      (drop-command-code exec)) "</command>"))
                   "</action></item>"
                   ))]))
            app-list))
          "</menu>"
          )))
      app-types))

    (string-append*
     (list
      "<menu id=\"root-menu\" label=\"Openbox 3\">"
      (string-append*
       (map (lambda (type)
              (string-append* (list "<menu id=\"" type "\"/>")))
            type-list))
      "</menu>"))
    "</openbox_menu>")))

(define (xml->file path output-xml)
  (define out
    (open-output-file path #:exists 'replace))
  (display output-xml out)
  (close-output-port out))

(xml->file "menu.xml"
           (processed-apps-info->xml (process-apps-info apps-info)))

(system "xmllint -o menu.xml --format menu.xml")

(define (extract-app-name path)
  (define text (path->string path))
  (define (last-element items)
    (car (take-right items 1)))
  (define items
    (string-split (last-element (string-split text "/")) "."))
  (car (take-right items 2)))

(define (insert-data-into-db application-list)
  (define conn
    (sqlite3-connect #:database "/home/heyanjie/.config/Minimalism/applications.db"))
  (define (iter-setting app-name setting-list)
    (when (not (null? setting-list))
      (define setting (car setting-list))
      (match setting
        [(SETTING entry key-value-pairs)
         (map
          (lambda (key-value-pair)
            (match key-value-pair
              [(list key value)
               (define query (string-append*
                              (list "INSERT INTO Application VALUES("
                                    QUOTE app-name QUOTE COMMA
                                    QUOTE entry QUOTE COMMA
                                    QUOTE key QUOTE COMMA
                                    QUOTE value QUOTE
                                    ");"
                                    )))
               (displayln query)
               (query-exec conn query)
               ]))
          key-value-pairs)])
      (iter-setting app-name (cdr setting-list))))
  (define (iter application-list)
    (when (not (null? application-list))
      (define app (car application-list))
      (match app
        [(APPLICATION path setting-list)
         (iter-setting (extract-app-name path) setting-list)])
      (iter (cdr application-list))))
  (iter application-list))

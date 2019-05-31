#lang racket

(require json)
(require db)

(define QUOTE "\"")
(define COMMA ",")

(define text (file->string "menu.json"))

(define menu
  (string->jsexpr text))

(define id 0)

(define (insert-data menu)
  (define (assign-id)
    (set! id (+ id 1))
    id)
  (define conn
    (sqlite3-connect #:database "/home/heyanjie/.config/Minimalism/menu.db"))
  (define (insert-one-row row-id parent-id name command action item_type)
    (define query (string-append*
                   (list "INSERT INTO Menu_Item VALUES("
                         QUOTE (number->string row-id) QUOTE COMMA
                         QUOTE (number->string parent-id) QUOTE COMMA
                         QUOTE name QUOTE COMMA
                         QUOTE command QUOTE COMMA
                         QUOTE action QUOTE COMMA
                         QUOTE item_type QUOTE
                         ");"
                         )))
    (displayln query)
    (query-exec conn query)
    )
  (define (insert-menu table parent-id)
    (if (and
         (hash-has-key? table 'name)
         (hash-has-key? table 'type)
         (hash-has-key? table 'items))
        (let [(row-id (assign-id))]
          (insert-one-row
           row-id
           parent-id
           (hash-ref table 'name)
           ""
           ""
           "Menu")
          (for/list ([item (hash-ref table 'items)])
            (insert-obj item row-id))
          )
        (error "wrong json format")))

  (define (insert-item table parent-id)
    (if (and
         (hash-has-key? table 'name)
         (hash-has-key? table 'type)
         (hash-has-key? table 'action)
         (hash-has-key? table 'command))
        (let [(row-id (assign-id))]
          (insert-one-row
           row-id
           parent-id
           (hash-ref table 'name)
           (hash-ref table 'command)
           (hash-ref table 'action)
           "Item"))
        (error "wrong json format")))

  (define (insert-obj table parent-id)
    (if (hash-has-key? table 'type)
        (match (hash-ref table 'type)
          ["Menu" (insert-menu table parent-id)]
          ["Item" (insert-item table parent-id)]
          [_ (error "wrong type")])
        (error "missing type field")))

  (define (insert-top-level table)
    (for/list ([item table])
      (insert-obj item 0)))
  (insert-top-level menu))

(insert-data menu)
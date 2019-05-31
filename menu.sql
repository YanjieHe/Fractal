create table Menu_Item(
	id INT NOT NULL,
	parent INT,
	name TEXT NOT NULL,
	command TEXT,
	action TEXT,
	item_type TEXT NOT NULL
);

create table Menu_Look(
	key TEXT NOT NULL,
	value TEXT NOT NULL
);

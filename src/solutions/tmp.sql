CREATE TABLE IF NOT EXISTS book
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name varchar(256) NOT NULL,
    created timestamp not null default(datetime('now')),
    created_by varchar(128) NOT NULL,
    modifed timestamp not null default(datetime('now')),
    modifed_by varchar(128) NOT NULL,
    currency varchar(3) NOT NULL
);

CREATE TABLE IF NOT EXISTS fx_rate
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    currency varchar(3) NOT NULL,
    rate float NOT NULL,
    created timestamp not null default(datetime('now'))
);

CREATE TABLE IF NOT EXISTS books_to_offers
(
    book_id int NOT NULL,
    offer_id int NOT NULL,

    FOREIGN KEY(book_id) REFERENCES book(id),
    FOREIGN KEY(offer_id) REFERENCES offer(id)
    --UNIQUE KEY(book_id, offer_id)
);

CREATE TABLE IF NOT EXISTS offer
(
    `id` INTEGER PRIMARY KEY AUTOINCREMENT,
    `price` int NOT NULL,
    `quantity` int NOT NULL,
    `type` VARCHAR(1) NOT NULL CHECK (`type` IN ('A', 'B'))
);

INSERT INTO fx_rate (`currency`, `rate`) VALUES ('RUR', 0.73);

CREATE TABLE tmp
(
    `name` VARCHAR(256),
    `value` INTEGER NOT NULL,

    PRIMARY KEY(`name`)
);

BEGIN TRANSACTION;
    INSERT INTO book
    (
        `name`,
        `created_by`,
        `modifed_by`,
        `currency`
    )
    VALUES
    (
        'Apples for RUR',
        'Sasha',
        'Sasha',
        'RUR'
    );

    INSERT INTO tmp VALUES('book', (SELECT last_insert_rowid()));

    INSERT INTO offer (`price`, `quantity`, `type`)
    VALUES (100, 1, 'A');
    INSERT INTO books_to_offers (book_id, offer_id)
    VALUES ((SELECT `value` FROM tmp WHERE `name` == 'book'), (SELECT last_insert_rowid()));

    INSERT INTO offer (`price`, `quantity`, `type`)
    VALUES (150, 2, 'A');
    INSERT INTO books_to_offers (book_id, offer_id)
    VALUES ((SELECT `value` FROM tmp WHERE `name` == 'book'), (SELECT last_insert_rowid()));

    INSERT INTO offer (`price`, `quantity`, `type`)
    VALUES (130, 1, 'B');
    INSERT INTO books_to_offers (book_id, offer_id)
    VALUES ((SELECT `value` FROM tmp WHERE `name` == 'book'), (SELECT last_insert_rowid()));

END TRANSACTION;

BEGIN TRANSACTION;
    INSERT INTO book
    (
        `name`,
        `created_by`,
        `modifed_by`,
        `currency`
    )
    VALUES
    (
        'Apples for USD',
        'Sasha',
        'Sasha',
        'USD'
    );

    INSERT INTO books_to_offers (book_id, offer_id)
    SELECT (SELECT last_insert_rowid()), `offer_id`
    FROM books_to_offers
    WHERE `book_id` == (SELECT `id` FROM book WHERE `name` == 'Apples for RUR');

    UPDATE tmp SET 'value' = (SELECT last_insert_rowid()) WHERE name == 'book';

    INSERT INTO offer (`price`, `quantity`, `type`)
    VALUES (25, 5, 'A');
    INSERT INTO books_to_offers (book_id, offer_id)
    VALUES ((SELECT `value` FROM tmp WHERE `name` == 'book'), (SELECT last_insert_rowid()));

    INSERT INTO offer (`price`, `quantity`, `type`)
    VALUES (300, 3, 'B');
    INSERT INTO books_to_offers (book_id, offer_id)
    VALUES ((SELECT `value` FROM tmp WHERE `name` == 'book'), (SELECT last_insert_rowid()));
END TRANSACTION;

DROP TABLE tmp;

SELECT book_name, bid_price, ask_price
FROM
(
    SELECT * FROM
    (
        SELECT book.id as book_id, book.name as book_name, max(price) as bid_price FROM books_to_offers
        LEFT JOIN book ON `book_id` == book.id
        LEFT JOIN offer ON `offer_id` == offer.id
        WHERE `type` == 'B'
        GROUP BY book.id
    ) as bid
    JOIN
    (
        SELECT book.id as book_id, min(price) as ask_price FROM books_to_offers
        LEFT JOIN book ON `book_id` == book.id
        LEFT JOIN offer ON `offer_id` == offer.id
        WHERE `type` == 'A'
        GROUP BY book.id
    ) as ask ON bid.book_id == ask.book_id
)

--ORDER BY `book_id`, offer.price;

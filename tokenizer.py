"""
This script take all the site referenced in the data set, analyses the words and create a reverse index of the world
"""

import psycopg2
import os
import time

from psycopg2.extras import register_composite
from collections import Counter

def get_pending_tokenize_row_ids(conn, batch_size = 100):
    with conn.cursor() as curs:
        curs.execute(
            """
            SELECT id FROM crawling WHERE state = 'PRETTY' LIMIT %s
            """, (batch_size,)
        )
        return [ tup[0] for tup in curs.fetchall() ]

def get_content_as_words(conn, id: int):
    with conn.cursor() as curs:
        curs.execute(
            """
            SELECT content FROM crawling WHERE id = %s
            """, (id,)
        )
        
        row = curs.fetchone()
        if row is None or row[0] is None:
            return []
    return row[0].split()

def compute_word_count(words: list[str]) -> dict[str,int]:
    return dict(Counter(words))

def get_reverse_row_from_word(conn, word):
    with conn.cursor() as curs:
        curs.execute(
            """
            SELECT * FROM reversed WHERE word = %s FOR UPDATE
            """, (word,)
        )
        
        reverse_row = curs.fetchone()
    if reverse_row is None:
        return (word, [])
    return reverse_row

def update_reverse_row(conn,word: str,reverse_entries: list[tuple[int,int]]):
    with conn.cursor() as curs:
        curs.execute(
            """
            INSERT INTO reversed VALUES (%s,%s::reverse_entry[]) ON CONFLICT (word) DO UPDATE SET reference = EXCLUDED.reference
            """,(word,reverse_entries,)
        )

def update_reverse_entries_from_words_count(conn, id: int, words_count: dict[str,int]):
    for current_word, current_word_count in words_count.items():
        reverse_row = get_reverse_row_from_word(conn,current_word)
        token = reverse_row[0]
        reverse_entries = reverse_row[1]
        new_reverse_entry = (id, current_word_count)
        
        found = False
        for i in range(len(reverse_entries)):
            reverse_entry = reverse_entries[i]
            crawling_row_id = reverse_entry[0]
            if crawling_row_id == id:
                reverse_entries[i] = new_reverse_entry
                found = True
            
        if not found:
            reverse_entries.append(new_reverse_entry)
        
        update_reverse_row(conn,current_word,reverse_entries)

def tokenize_one(conn, pending_id: int):
    content_words: list[str] = get_content_as_words(conn,pending_id)
    word_count: dict[str, int] = compute_word_count(content_words)
    update_reverse_entries_from_words_count(conn,pending_id,word_count)

def update_crawled_tokenized(conn, pending_ids: list[int]):
    with conn.cursor() as curs:
        curs.execute(
            """
            UPDATE crawling SET state = 'TOKENIZED' WHERE id = ANY(%s)
            """, (pending_ids,)
        )

def tokenize_all(conn, pending_ids: list[int]):
    for id in pending_ids:
        print(f"Tokenizing id: {id}")
        tokenize_one(conn, id)
    print(f"*** TOKENIZED ***")

def tokenizer(conn):
    while True:
        time.sleep(10.0)
        with conn:
            pending_ids: list[int] = get_pending_tokenize_row_ids(conn)
            if len(pending_ids) == 0 or pending_ids is None:
                continue
            tokenize_all(conn, pending_ids)
            update_crawled_tokenized(conn,pending_ids)
    

def main():
    conn = psycopg2.connect(
    dbname=os.environ.get("DB_NAME", "gogole"),
    user=os.environ.get("DB_USER", "developer"),
    password=os.environ["POSTGRES_PASSWORD"],  # Required, no default
    host=os.environ.get("DB_HOST", "db")
    )
    register_composite('reverse_entry', conn,globally=True)
    
    try:
        tokenizer(conn)
    finally:
        conn.close()
    

if __name__ == "__main__":
    main()
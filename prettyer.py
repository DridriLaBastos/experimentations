"""
The goal of this script is to remove non pertinent word from page web to decrease the amount of data to be stored and analysed
"""

import argparse
import psycopg2
import os

from redis import Redis

choices = ("database", "redis")

parser = argparse.ArgumentParser(description="Removes unrelevent world from web pages")
parser.add_argument("--source", choices=choices, default=choices[0])
args = parser.parse_args()

INPUT_BATCH_SIZE = 10

def prettier(url: str, text: str):
    old_size = len(text)
    print(f"Working on {url}")
    if old_size == 0:
        print(f"*** EMPTY ***")
        return text
    new_string = ' '.join([content for content in text.split(' ') if content not in ['mais', 'où', 'et', 'donc', 'or', 'ni', 'car', 'le', 'la', 'les', 'je', 'tu', 'il', 'elle', 'on', 'nous', 'vous', 'ils', 'elles'] ])
    new_size = len(new_string)
    
    print(f"\t{old_size} -> {new_size} : reduction {(1 - new_size / old_size) * 100:.2}%")
    return new_string

def database_fetch_batch(conn, input_batch_size=INPUT_BATCH_SIZE):
    with conn.cursor() as curs:
        curs.execute(
            """
            SELECT url, content FROM crawling WHERE state = 'NOT_READY' LIMIT %s
            """, (input_batch_size,)
        )
        
        return curs.fetchall()

def send_input_batch(output, input_batch: list[tuple[str,str]]):
    with output.cursor() as curs:
        for url,content in input_batch:
            curs.execute(
                """
                INSERT INTO crawling VALUES (%s,%s,'PRETTY') ON CONFLICT (url) DO UPDATE SET content=EXCLUDED.content, state=EXCLUDED.state
                """,(url,content,)
            )

def prettier_from_database(output):
    # In that case the output database is also the input
    input = output
    while True:
        input_batch: list[tuple[str,str]] = database_fetch_batch(input)
        processed_batch = [(url, prettier(url,content)) for url, content in input_batch]
        send_input_batch(output,processed_batch)
        input.commit()
        

def prettier_from_redis(output):
    pass

def main():
    output = psycopg2.connect(
            dbname=os.environ.get("DB_NAME", "gogole"),
            user=os.environ.get("DB_USER", "developer"),
            password=os.environ["POSTGRES_PASSWORD"],  # Required, no default
            host=os.environ.get("DB_HOST", "db")
        )

    source: str = args.source
    
    with output:
        if source == "redis":
            prettier_from_redis(output)
        
        elif source == "database":
            prettier_from_database(output)
    
    assert False

if __name__ == "__main__":
    main()
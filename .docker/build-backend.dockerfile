FROM python:3.14.7

ARG URL="0.0.0.0"
ARG PORT="8000"

RUN python -m pip install django python-dotenv psycopg

CMD [ "python", "manage.py", "runserver", "0.0.0.0:8001" ]

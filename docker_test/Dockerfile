FROM python:latest

RUN echo 'Building a new image from existing'

ENV FOOR=baar

WORKDIR /mnt/app
COPY . .

# CMD [ "ls", "-fl", "/" ]
RUN pip install --no-cache-dir -r requirements.txt

CMD [ "/bin/sh"]

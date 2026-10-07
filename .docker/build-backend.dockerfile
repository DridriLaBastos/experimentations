FROM golang:1.25

RUN go install -v github.com/go-delve/delve/cmd/dlv@latest

CMD [ "go", "run", "." ]

package main

import (
	"log/slog"
	"net"
	"net/http"
	"strings"

	"github.com/go-chi/chi/v5"
	"github.com/go-chi/chi/v5/middleware"
)

func main() {
	router := chi.NewRouter()

	router.Use(middleware.Heartbeat("/health"))
	router.Use(middleware.Logger)
	router.Get("/editor", func(w http.ResponseWriter, r *http.Request) {
		http.Redirect(w,r,"http://localhost:3000", http.Redi)
	})

	err := http.ListenAndServe(":8080", router)

	if err != nil {
		slog.Error(err.Error())
	}
}

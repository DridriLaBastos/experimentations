from django.urls import path

from . import views

app_name="connection"

urlpatterns = [
    path("", views.connect, name="home"),
    path("connect", views.try_connect, name="connect"),
    path("disconnect", views.try_disconnect, name="disconnect")
]

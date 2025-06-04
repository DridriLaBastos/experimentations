from django.urls import path

from . import views

urlpatterns = [
    path("", views.connect),
    path("connect", views.try_connect, name="connect"),
]

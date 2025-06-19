from django.urls import path

from . import views

app_name = "explorer"

urlpatterns = [
    path("", views.explore, name="explore")
]

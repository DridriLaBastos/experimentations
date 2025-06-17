from django.urls import path

from . import views

app_name = "explorer"

urlpatterns = [
    path("explore", views.explore, name="explore")
]

from django.urls import path, include
from django.contrib.auth import views as auth_views

from . import views

urlpatterns = [
    path("accounts/", include("django.contrib.auth.urls")),
    path("accounts/profile/", views.profile, name="profile"),
    path("accounts/profile/editor", views.editor,name="open_editor" )
]
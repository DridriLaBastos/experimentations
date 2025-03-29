from django.urls import path

from . import views

# Use a path like /docs/<docid> to access a document
# https://docs.djangoproject.com/en/4.2/intro/tutorial03/#writing-more-views
urlpatterns = [
    path("",views.edition,name="default_edition"),
    path("<int:document_id>/",views.edition,name="edition")
]

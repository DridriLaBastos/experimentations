from django.db import models

# Create your models here.
class Document(models.Model):
    name     = models.CharField("document_name",max_length=256)
    creation = models.DateTimeField("creation_date")
    modif    = models.DateTimeField("last_modified")
    mddata   = models.TextField("markedown_content", default="")
    htmldata = models.TextField("html_content", default="")
    
    class Meta:
        db_table='documents'
    
    def __str__(self):
        return f"'{self.name}'"
    
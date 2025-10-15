```c++

int gluBuild2DMipmaps(GLenum target, GLint internalFormat, GLint width, GLint height, GLenum format, GLenum type, void *texels);

```

Parametri su isti kao kod [[glTexImage2D]], osim što se ne navodi nivo (jer formira sve nivoe), i border.

Cak i kada dimenzije nisu stepen dvojke, ova funkcija normalizuje dimenzije bitmape.
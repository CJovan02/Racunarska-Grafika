
Potreban nam je jedan niz koordinata za crtanje normala. Sastoji se od koordinata vertex-a i koordinata normale za dati vertex.
```c++
vector<tuple<double, double, double, double, double, double>> normals;
```

Svaki put kada definisemo vertex i normalu dodamo je u niz:
```c++
normals.emplace_back(vx, vy, vz, nx, ny, nz);
```

Kada zavrsimo sa crtanjem 3D tela pozovemo ovu funkciju i prosledimo niz koordinata:
```c++
void CGLRenderer::DrawNormals(vector<tuple<double, double, double, double, double, double>> normals, double scale)
{
	if (drawNormals)
	{
		glColor3d(1, 1, 1);
		glBegin(GL_LINES);
		{
			for (const auto& normal : normals)
			{
				double vx, vy, vz, nx, ny, nz;
				tie(vx, vy, vz, nx, ny, nz) = normal;

				glVertex3d(vx, vy, vz);
				glVertex3d(vx + nx * scale, vy + ny * scale, vz + nz * scale);
			}
		}
		glEnd();
	}
}
```
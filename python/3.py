import math

# --- Notre classe Vector, maintenant plus complète ---
class Vector:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    # Addition de vecteurs (ex: P = O + D*t)
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y, self.z + other.z)

    # Soustraction de vecteurs (ex: oc = O - C)
    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)

    # Multiplication par un nombre (ex: D * t)
    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar, self.z * scalar)

    # Produit scalaire
    def dot(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z
    
    # Calcule la longueur au carré (plus rapide que length)
    def length_squared(self):
        return self.dot(self)

    # Renvoie une version normalisée (longueur de 1) du vecteur
    def normalize(self):
        length = math.sqrt(self.length_squared())
        if length == 0:
            return Vector(0, 0, 0)
        return Vector(self.x / length, self.y / length, self.z / length)

# --- Fonction d'intersection (renvoie t ou -1.0) ---
def hit_sphere(center, radius, ray_origin, ray_direction):
    oc = ray_origin - center
    a = ray_direction.dot(ray_direction)
    b = 2.0 * oc.dot(ray_direction)
    c = oc.dot(oc) - radius * radius
    discriminant = b*b - 4*a*c

    if discriminant < 0:
        return -1.0
    else:
        sqrt_discriminant = math.sqrt(discriminant)
        t1 = (-b - sqrt_discriminant) / (2 * a)
        t2 = (-b + sqrt_discriminant) / (2 * a)

        if t1 > 0:
            if t2 > 0: return min(t1, t2)
            else: return t1
        elif t2 > 0:
            return t2
        
        return -1.0 # Les deux sont négatifs

# --- Programme principal ---

# Image
image_width = 400
image_height = 200

# Scène
camera_origin = Vector(0, 0, 0)
# J'ai reculé la sphère (z=-3) pour mieux voir l'ombre
sphere_center = Vector(0, 0, -3) 
sphere_radius = 1.0

# NOUVEAU: La source de lumière (en haut à gauche)
light_position = Vector(-5, 5, 0)

# NOUVEAU: Nos couleurs
color_sky = (135, 206, 235)     # Bleu ciel
color_lit = (255, 0, 0)         # Rouge vif (éclairé)
color_shadow = (0, 0, 0)      # Rouge sombre (ombre)

# NOUVEAU: Un tout petit nombre pour éviter les problèmes d'auto-intersection
epsilon = 0.001

# Rendu
with open("sphere_shadow.ppm", "w") as f:
    f.write("P3\n")
    f.write(f"{image_width} {image_height}\n")
    f.write("255\n")

    for y in range(image_height):
        for x in range(image_width):
            
            # Coordonnées du pixel sur l'écran virtuel
            # (J'ai corrigé l'axe Y pour qu'il soit plus intuitif)
            u = (x - image_width / 2) / (image_width / 2)
            v = (image_height / 2 - y) / (image_height / 2) # y=0 est en HAUT
            
            aspect_ratio = image_width / image_height
            u *= aspect_ratio
            
            # On normalise la direction du rayon (important)
            ray_direction = Vector(u, v, -1).normalize()

            # On récupère la distance 't'
            t = hit_sphere(sphere_center, sphere_radius, camera_origin, ray_direction)
            
            if t > 0:
                # --- ON A TOUCHÉ LA SPHÈRE ---
                # C'est ici que la nouvelle logique intervient
                
                # 1. Calculer le point d'impact P
                hit_point = camera_origin + (ray_direction * t)
                
                # 2. Définir le rayon d'ombre (de P vers la lumière)
                shadow_ray_dir = light_position - hit_point
                
                # 3. On décale le point P pour éviter qu'il se touche lui-même
                shadow_ray_origin = hit_point + (shadow_ray_dir.normalize() * epsilon)
                
                # 4. On lance le rayon d'ombre
                t_shadow = hit_sphere(sphere_center, sphere_radius, shadow_ray_origin, shadow_ray_dir)

                if t_shadow > 0:
                    # Le pixel est dans l'ombre
                    color = color_shadow
                else:
                    # Le pixel est ÉCLAIRÉ
                    N = (hit_point - sphere_center).normalize()
                    L = (light_position - hit_point).normalize()
                    
                    # Calcul de la luminosité (on s'assure qu'elle est >= 0)
                    brightness = max(0, N.dot(L)) 
                    
                    # On applique la luminosité à chaque composante (R, G, B)
                    r = color_lit[0] * brightness
                    g = color_lit[1] * brightness
                    b = color_lit[2] * brightness
                    
                    # On assigne le nouveau tuple de couleur
                    color = (r, g, b)
            else:
                # --- ON A RATÉ LA SPHÈRE ---
                color = color_sky
            
            # Écrire la couleur (convertie en entiers)
            f.write(f"{int(color[0])} {int(color[1])} {int(color[2])}\n")

print("Image 'sphere_shadow.ppm' générée !")

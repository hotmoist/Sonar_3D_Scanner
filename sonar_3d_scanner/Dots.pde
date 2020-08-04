class Dots {
  float x, y, z;
  
  Dots(float x, float y, float z) {
    this.x = x;
    this.y = y;
    this.z = z;
  
  }

  void draw_dot() {

    pushMatrix();    
    translate(x, y, z);
    noStroke();
    fill(0, 255, 0);
    box(10);
    popMatrix();
    
  }
}

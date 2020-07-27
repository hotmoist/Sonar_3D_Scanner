class Dots {
  float x, y, z;
  float dot_angle;

  Dots(float x, float y, float z, float angle) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.dot_angle = angle;
   
  }

  void draw_dot() {
    float r = dist(this.x, this.z, 0, 0);

    pushMatrix();    
    translate( r*cos(radians(dot_angle + angle)), this.y, r*sin(radians(dot_angle + angle)));
    noStroke();
    fill(0, 255, 0);
    box(5);
  //  translate(-r*cos(radians(dot_angle + angle)), -this.y, -r*sin(radians(dot_angle + angle)));
    /*
    if (t_case == 0) {
     angle += 1;
     }
     */
    popMatrix();
  }
}

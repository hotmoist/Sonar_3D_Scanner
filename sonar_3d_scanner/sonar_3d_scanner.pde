import java.util.*;
import processing.serial.*;

// actual height rises 0.07cm per 360 degree turn
// one step table turn is 1.8 degree (radian(1.8))
// closest distance between table and sensor is near 7 cm 


Serial port;

ArrayList<Dots> list = new ArrayList<Dots>();
int t_case = 0;


String val;
float[] serialVal = new float[3];
int serialCount = 0;
float angle = 0;
float distance = 0;
float h = 0;

// debug
int count = 0;


final float DISTANCE_CENTER = 11; //distance of sensor~~center

float x = 0;
float y = 0;
float z = 0;

void setup() {
  size(1000, 1000, P3D);
  background(0);
  printArray(Serial.list());
  port = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  translate(500, 500);
  background(0);
  stroke(255);
  fill(0, 255, 0);

  // draw line for center coordinate plane

  line(0, -500, 0, 0, 500, 0);
  line(-500, 0, 0, 500, 0, 0);
  line(0, 0, 500, 0, 0, -500);
  text("(0, -500, 0)", 0, -480, 0);
  text("(0, 500, 0)", 0, 480, 0);
  text("(500, 0, 0)", 430, 15, 0);
  text("(-500, 0, 0)", -500, 15, 0);

  if (t_case == 0) {

    /*
     * case 0 codes
     * this code is for to make boxes of measured points.
     *  
     */


    if (port.available() > 0) {    // serial communication with arduino

      // three variable will be sended : distance, angle, height

      val = port.readStringUntil('\n');

      if (serialCount < 3 && val != null) {
        serialVal[serialCount] = Float.parseFloat(val);
        serialCount++;
      }

      if (serialCount == 3) {
        distance = serialVal[0];
        angle =serialVal[1];
        h = serialVal[2];

        println("distance : " + distance + " angle : " + angle + " height : " + h);

        serialCount = 0;
        
        count++;
      }
    }

    /****** noise deleting code*****/
    // noise deleting code
    // need more idea
    //if (distance % 1 < 0.5) {

    //  distance = (distance / 1);
    //} else if (distance % 1 >= 0.5) {

    //  distance = (distance / 1) + 0.5;
    //}

    /******************************/

    float dist =(DISTANCE_CENTER - distance) *50;

    /***** scan test code******/
    //--------------------------
    if (count == 200) {
      t_case = 1;
    }
    //---------------------------

    if (h == 1000 ) { 
      //when there is nothing to scan or either scan is finished
      //end condition
      // arduino will send info as angle = 0, distance = 0, height = 1000

      t_case = 1;
    }

    if ( dist > 0) {

      x = dist*cos(radians(angle));
      y = h;
      z= dist*sin(radians(angle));

      list.add(new Dots(x, y, z));

      for (int i = 0; i < list.size(); i++) {
        list.get(i).draw_dot();
      }

      delay(1);
    } else {

      for (int i = 0; i < list.size(); i++) {

        list.get(i).draw_dot();
      }
    }

    delay(1);

    
  }

  if (t_case == 1) {
    /*
     * case 1 codes
     * this codes is for rendering all the points to make scanned object look smoother.
     * it is like anti-aliasing
     */


    // beginShape(TRIANGLE_STRIP);
    for (int i = 0; i < list.size(); i++) {
      //  beginShape(TRIANGLE_STRIP);

      //  vertex(list.get(i).x*t, list.get(i).y*t, list.get(i).z*t);
      // vertex(list.get(i).x*t, list.get(i).y*t, (list.get(i).z+1)*t);

      list.get(i).draw_dot();
    }
    // endShape();

    camera(mouseX*2, 0, mouseY*2, 500, 500, 0, 100, 100, 100 );
  }
}

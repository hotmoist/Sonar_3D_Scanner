class Button {

  private int x, y;
  private int w, h;
  private String text; 
  private int text_len;
  private int text_x, text_y;
  private final int text_size = 10;

  public Button(String text) {

    this.text = text;
    this.text_len = text.length() *10;

    w = text_len + 20;
    h = text_size + 20;
  }


  public Button (int x, int y, String text) {
    this.x = x;
    this.y = y-5;
    this.text = text;
    this.text_len = text.length() *10;

    text_x = x + 8;
    text_y = y - 15;
    w = text_len + 20;
    h = text_size + 20;
  }


  void createButton(int x, int y) {

    this.x = x;
    this.y = y-5;
    text_x = x + 8;
    text_y = y + 20;

    pushMatrix();
    stroke(0, 255, 0);
    fill(0);
    rect(x, y, w, h);
    textSize(text_size + 10);
    fill(0, 255, 0);
    text(text, text_x, text_y);
    popMatrix();
  }

  void createButton() {

    pushMatrix();
    stroke(0, 255, 0);
    fill(0);
    rect(x, y, w, h);
    textSize(text_size + 10);
    fill(0, 255, 0);
    text(text, text_x, text_y);
    popMatrix();
  }

  private boolean onButton() {


    if (((mouseX-500) >= x && (mouseX-500) <= x+ w) && ((mouseY-800) >= y && (mouseY-800) <= y + h)) {
      return true;
    } else {
      return false;
    }
  }

  boolean click() {

    if (onButton() && mousePressed) {
      return true;
    } else {
      return false;
    }
  }
}

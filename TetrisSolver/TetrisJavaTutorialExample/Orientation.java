package tetris.ai;

public class Orientation {
  public Point[] squares = new Point[4];
  public int minX;
  public int maxX;
  public int maxY;
  
  public Orientation() {
    for(int i = 0; i < 4; i++) {
      squares[i] = new Point();
    }
  }
}

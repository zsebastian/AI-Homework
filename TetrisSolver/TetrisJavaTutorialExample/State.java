package tetris.ai;

public class State {
  public int x;
  public int y;
  public int rotation;
  public int visited;
  public State predecessor; 
  public State next;
  
  public State(int x, int y, int rotation) {
    this.x = x;
    this.y = y;
    this.rotation = rotation;
  }
}

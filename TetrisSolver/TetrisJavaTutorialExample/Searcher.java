package tetris.ai;

public class Searcher {
  
  private static int globalMark = 1;
  
  private State[][][] states;
  private Queue queue = new Queue();
  private ISearchListener searchListener;
  private IChildFilter positionValidator;
  
  public Searcher(
      ISearchListener searchListener, IChildFilter positionValidator) {
    this.searchListener = searchListener;
    this.positionValidator = positionValidator;
    createStates();
  }
  
  private void createStates() {
    states = new State[AI.PLAYFIELD_HEIGHT][AI.PLAYFIELD_WIDTH][4];
    for(int y = 0; y < AI.PLAYFIELD_HEIGHT; y++) {
      for(int x = 0; x < AI.PLAYFIELD_WIDTH; x++) {        
        for(int rotation = 0; rotation < 4; rotation++) { 
          states[y][x][rotation] = new State(x, y, rotation);
        }
      }
    }
  }
  
  private void lockTetrimino(
      int[][] playfield, int tetriminoType, int id, State state) {
    Point[] squares = Tetriminos.ORIENTATIONS[tetriminoType][state.rotation]
        .squares;
    for(int i = 0; i < 4; i++) {
      Point square = squares[i];
      int y = state.y + square.y;
      if (y >= 0) {
        playfield[y][state.x + square.x] = tetriminoType;
        playfield[y][AI.PLAYFIELD_WIDTH]++;
      }
    }
    searchListener.handleResult(playfield, tetriminoType, id, state);
    for(int i = 0; i < 4; i++) {
      Point square = squares[i];
      int y = state.y + square.y;
      if (y >= 0) {
        playfield[y][state.x + square.x] = Tetriminos.NONE;
        playfield[y][AI.PLAYFIELD_WIDTH]--;
      }
    }
  }
  
  // returns true if the position is valid even if the node is not enqueued
  private boolean addChild(int[][] playfield, int tetriminoType, int mark, 
      State state, int x, int y, int rotation) {

    Orientation orientation = Tetriminos.ORIENTATIONS[tetriminoType][rotation];
    if (x < orientation.minX || x > orientation.maxX || y > orientation.maxY) {
      return false;
    }

    State childNode = states[y][x][rotation];
    if (childNode.visited == mark) {
      return true;
    }

    Point[] squares = orientation.squares;
    for(int i = 0; i < 4; i++) {
      Point square = squares[i];
      int playfieldY = y + square.y;
      if (playfieldY >= 0 
          && playfield[playfieldY][x + square.x] != Tetriminos.NONE) {
        return false;
      }
    }
    
    if (positionValidator != null && !positionValidator.validate(
        playfield, tetriminoType, x, y, rotation)) {
      return true;
    }

    childNode.visited = mark;
    childNode.predecessor = state;
        
    queue.enqueue(childNode);   
    return true; 
  }  
  
  public boolean search(int[][] playfield, int tetriminoType, int id) {

    int maxRotation = Tetriminos.ORIENTATIONS[tetriminoType].length - 1;

    int mark = globalMark++;

    if (!addChild(playfield, tetriminoType, mark, null, 5, 0, 0)) {
      return false;
    }    

    while(queue.isNotEmpty()) {
      State state = queue.dequeue();

      if (maxRotation != 0) {
        addChild(playfield, tetriminoType, mark, state, state.x, state.y, 
            state.rotation == 0 ? maxRotation : state.rotation - 1);
        if (maxRotation != 1) {
          addChild(playfield, tetriminoType, mark, state, state.x, state.y, 
              state.rotation == maxRotation ? 0 : state.rotation + 1);
        }
      }

      addChild(playfield, tetriminoType, mark, state, 
          state.x - 1, state.y, state.rotation);
      addChild(playfield, tetriminoType, mark, state, 
          state.x + 1, state.y, state.rotation);

      if (!addChild(playfield, tetriminoType, mark, state,
          state.x, state.y + 1, state.rotation)) {
        lockTetrimino(playfield, tetriminoType, id, state);
      }
    }

    return true;
  }
}

package tetris.ai;

import java.util.*;

public final class Tetriminos {
  
  public static final int NONE = -1;
  public static final int T = 0;
  public static final int J = 1;
  public static final int Z = 2;
  public static final int O = 3;
  public static final int S = 4;
  public static final int L = 5;
  public static final int I = 6;
  
  public static final int[][][][] PATTERNS = {
    { { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, },    // Td (spawn)
      { {  0, -1 }, { -1,  0 }, {  0,  0 }, {  0,  1 }, },    // Tl    
      { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  0, -1 }, },    // Tu
      { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, }, }, // Tr   

    { { { -1,  0 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, },    // Jd (spawn)
      { {  0, -1 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, },    // Jl
      { { -1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },    // Ju
      { {  0, -1 }, {  1, -1 }, {  0,  0 }, {  0,  1 }, }, }, // Jr   

    { { { -1,  0 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, },    // Zh (spawn) 
      { {  1, -1 }, {  0,  0 }, {  1,  0 }, {  0,  1 }, }, }, // Zv   

    { { { -1,  0 }, {  0,  0 }, { -1,  1 }, {  0,  1 }, }, }, // O  (spawn)   

    { { {  0,  0 }, {  1,  0 }, { -1,  1 }, {  0,  1 }, },    // Sh (spawn)
      { {  0, -1 }, {  0,  0 }, {  1,  0 }, {  1,  1 }, }, }, // Sv   

    { { { -1,  0 }, {  0,  0 }, {  1,  0 }, { -1,  1 }, },    // Ld (spawn)
      { { -1, -1 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, },    // Ll
      { {  1, -1 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },    // Lu
      { {  0, -1 }, {  0,  0 }, {  0,  1 }, {  1,  1 }, }, }, // Lr      

    { { { -2,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, },    // Ih (spawn)    
      { {  0, -2 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, }, }, // Iv      
  };
  
  
  public static final Orientation[][] ORIENTATIONS;
  
  static {
    ORIENTATIONS = new Orientation[PATTERNS.length][];
    for(int i = 0; i < PATTERNS.length; i++) {
      List<Orientation> tetriminos = new ArrayList<>();
      for(int j = 0; j < PATTERNS[i].length; j++) {
        Orientation tetrimino = new Orientation();
        tetriminos.add(tetrimino);
        int minX = Integer.MAX_VALUE;
        int maxX = Integer.MIN_VALUE;
        int maxY = Integer.MIN_VALUE;
        for(int k = 0; k < 4; k++) {
          int[] p = PATTERNS[i][j][k];
          tetrimino.squares[k].x = p[0];
          tetrimino.squares[k].y = p[1];
          minX = Math.min(minX, p[0]);
          maxX = Math.max(maxX, p[0]);
          maxY = Math.max(maxY, p[1]);          
        }
        tetrimino.minX = -minX;
        tetrimino.maxX = AI.PLAYFIELD_WIDTH - maxX - 1;
        tetrimino.maxY = AI.PLAYFIELD_HEIGHT - maxY - 1;
      }
      ORIENTATIONS[i] = new Orientation[tetriminos.size()];
      tetriminos.toArray(ORIENTATIONS[i]);
    }
  }
  
  private Tetriminos() {    
  }
}

struct SFCGamepad : Expansion {
  enum : uint {
    Up, Down, Left, Right, B, A, Y, X, L, R, Select, Start,
  };

  SFCGamepad();

  auto data1() -> bool;
  auto data2() -> uint5;
  auto write(uint3 data) -> void;

private:
  bool latched;
  uint counter;

  boolean b, y, select, start;
  boolean up, down, left, right;
  boolean a, x, l, r;
};

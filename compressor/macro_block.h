#ifndef MACRO_BLOCK_H
#define MACRO_BLOCK_H

class MacroBlock {
  public:
    MacroBlock(size_t width, size_t height);
    ~MacroBlock();


    /// TODO: ADD SET DATA FIELD
    /// TODO: ADD GET DATA FIELD

    size_t _width;
    size_t _height;
    float** _blockData;

    bool check_if_is_complete();

    int _numberOfInsertions = 0;
};



#endif //MACRO_BLOCK_H

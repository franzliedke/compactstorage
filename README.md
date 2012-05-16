# compactstorage

A C++ library for creating a compact bitstream made up of multiple different variables of variable length and different types overlapping to save space.

This is especially useful in combination with techniques like QR codes, where it can be useful to decrease the space needed by data in order to keep the codes small.

## License

This library is licensed under the [GNU Lesser General Public License (LGPL)](http://www.gnu.org/licenses/lgpl.html).

## Usage example

    #include <iostream>

    #include "compactstorage.h"

    using namespace std;

    int main(int argc, char** argv)
    {
      CompactStorage storage;
  
      // Write data
      storage.writeBool(true);
      storage.writeBool(false);
      storage.writeInt(25, 7);
      
      // Display the compacted data
      storage.dump();
      
      // Read it again
      storage.reset();
      cout << storage.readBool() << endl;
      cout << storage.readBool() << endl;
      cout << storage.readInt(7) << endl;
      
      return 0;
    }


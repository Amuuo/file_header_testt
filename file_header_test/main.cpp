#include "main.h"





vector<FileHeader> patterns;



int main(){

  

  ifstream       header_file;
  string         current_line;
  //string         current_word;

  header_file.open("file_headers.txt");
  getline(header_file, current_line);


  while (!header_file.eof())
  {
    getline(header_file, current_line);
    istringstream iss{current_line};
    cout << "current line: " << current_line << endl;
    while (iss)
    {
      string current_word;
      iss >> current_word;
      cout << "current word: " << current_word << endl;
      patterns.push_back(current_word);

      iss >> current_word;

      cout << "current word: " << current_word << endl;
      if (current_word == "[")
      {
        vector<string> tmp_vec;
        iss >> current_word;
        cout << "\n\tHEADER: ";
        while (current_word != "]")
        {
          tmp_vec.push_back(current_word);
          cout << tmp_vec.back() << " ";
          iss >> current_word;
        }
        cout << endl;
        patterns.back().get_header_byte_array(tmp_vec);
        iss >> current_word;
      }
      /*
      else
      {
        cout << "\ncurrent word: " << current_word;
        cout << "\nSignature file formatting error... Exiting....";
        exit(1);
      }
      */
      if (current_word == "[")
      {
        vector<string> tmp_vec;
        iss >> current_word;
        cout << "\n\tFOOTER: ";
        while (current_word != "]")
        {
          tmp_vec.push_back(current_word);
          cout << tmp_vec.back() << " ";
          iss >> current_word;
        }
        cout << endl;
        patterns.back().get_footer_byte_array(tmp_vec);
      }
      iss >> current_word;
      patterns.back().max_carve_size = stol(current_word);
      while (iss) iss.get();
      cout << "carve_size: " << patterns.back().max_carve_size << endl;
    }
  }

//#ifdef DEBUG
  for (auto pattern : patterns)
  {
    cout << "\nFile extension: " << pattern.file_extension;
    cout << "\nHeader signature: ";

    for (auto byte : pattern.header_byte_array)
      cout << setw(2) << setfill('0') << uppercase << hex << (int)byte << " ";

    cout << "\nFooter signature: ";

    if (!pattern.footer_byte_array.empty())
    {
        for (auto byte : pattern.footer_byte_array)
            cout << setw(2) << uppercase << hex << (int)byte << " ";

    }
    cout << "\nMax Carve Size: " << dec << pattern.max_carve_size << endl;
  }
//#endif // DEBUG

  ifstream testFile{"testinput.txt"};
  vector<uint8_t> test_array;

  uint8_t current_byte;
  
  while (!testFile.eof()) {
    
    current_byte = testFile.get();
    
    if (current_byte != '\n')
      test_array.push_back(current_byte);    
  }

  PatternMatcher PM{patterns};
  PM.find_matches(&test_array[0], test_array.size());

  for (int i = 0; i < PM.positions.size(); i++)
  {
    printf("\n%s: ", patterns[i].file_extension.c_str());
    for (auto header : patterns[i].header_byte_array) {
      printf("%x ", (int)header);
    }    
    printf("\n");
    for (auto itr = PM.positions[i].begin(); itr != PM.positions[i].end(); ++itr)
      printf("\t-%d\n", (int)*itr - (patterns[i].header_byte_array.size() - 1));
    printf("\n");
  }
  return 0;
}
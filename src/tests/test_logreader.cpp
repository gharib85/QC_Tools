

#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../libcatnip/io/file_readers/logreader.hpp"
#include "../libcatnip/io/io.hpp"

using namespace catnip;
using namespace std;

int main(void) {

  cout << "Testing: LogReader Constructor" << endl;
  { LogReader lr("file.log"); }

  cout << "Testing: LogReader read energies" << endl;
  {

    string test_file = "testfile_energies.log";
    {
      ofstream fid;
      fid.open(test_file);
      fid << "Test log file contains sections that the log reader";
      fid << " will search and read from." << endl;
      fid << " Alpha  occ. eigenvalues --  -10.19871 -10.19791  -0.76206  -0.58448  -0.47269" << endl; 
      fid << " Alpha  occ. eigenvalues --   -0.42268  -0.36322  -0.27597" << endl; 
      fid << " Alpha virt. eigenvalues --   -0.00988   0.02500   0.03257   0.04671   0.06946" << endl;
      fid << " Alpha virt. eigenvalues --    0.08668   0.08746   0.12753   0.15670   0.17639" << endl; 
      fid.close();
    }

    LogReader lr(test_file);
    lr.read();

    Eigen::VectorXd orbital_energies = lr.getOE("Alpha");
    
    set<double> correct_orbital_energies = { 
    -10.19871, -10.19791,  -0.76206, -0.58448, -0.47269, 
     -0.42268,  -0.36322,  -0.27597, 
     -0.00988,   0.02500,   0.03257,   0.04671,  0.06946,
      0.08668,   0.08746,   0.12753,   0.15670,  0.17639};

    vector<bool> found_energy(correct_orbital_energies.size(),false); 
    for( int index = 0; index < orbital_energies.size();++index){
      if( correct_orbital_energies.count(orbital_energies(index))){
        found_energy.at(index) = true;
      }
      cout << "Found Energy " << orbital_energies(index);
      cout << " " << found_energy.at(index) << endl;
    }

    for ( const bool & found : found_energy){
      assert(found);
    }
  }

  cout << "Testing: LogReader read coordinates" << endl;
  {
    string test_file = "testfile_coords.log";
    {
      ofstream fid;
      fid.open(test_file);
      fid << "Center     Atomic      Atomic" << endl;
      fid << " Number     Number      Type              X           Y           Z" << endl;
      fid << " ---------------------------------------------------------------------" << endl;
      fid << "    1          6             0        0.672749    0.000000    5.000000" << endl;
      fid << "    2          6             0       -0.672749    0.000000    5.000000" << endl;
      fid << "    3          1             0        1.242623    0.162327    5.920604" << endl;
      fid << "    4          1             0        1.242623   -0.162327    4.079396" << endl;
      fid << "    5          1             0       -1.242623    0.162327    5.920604" << endl;
      fid << "    6          1             0       -1.242623   -0.162327    4.079396" << endl;
      fid << "-----------------------------------";
      fid << "----------------------------------" << endl;
      fid.close();
    }

    LogReader lr(test_file);
    lr.read();

    vector<vector<double>> atomic_coords = lr.getCoords();
 
    cout << atomic_coords.at(0).at(0) << " ";
    cout << atomic_coords.at(1).at(0) << " ";
    cout << atomic_coords.at(2).at(0) << " ";
    assert(atomic_coords.at(0).at(0) == 0.672749); 
    assert(atomic_coords.at(1).at(0) == 0.000000); 
    assert(atomic_coords.at(2).at(0) == 5.000000);
    cout << "Atom 1 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(1) << " ";
    cout << atomic_coords.at(1).at(1) << " ";
    cout << atomic_coords.at(2).at(1) << " ";
    assert(atomic_coords.at(0).at(1) == -0.672749); 
    assert(atomic_coords.at(1).at(1) == 0.0000000); 
    assert(atomic_coords.at(2).at(1) == 5.0000000); 
    cout << "Atom 2 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(2) << " ";
    cout << atomic_coords.at(1).at(2) << " ";
    cout << atomic_coords.at(2).at(2) << " ";
    assert(atomic_coords.at(0).at(2) == 1.242623); 
    assert(atomic_coords.at(1).at(2) == 0.162327); 
    assert(atomic_coords.at(2).at(2) == 5.920604); 
    cout << "Atom 3 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(3) << " ";
    cout << atomic_coords.at(1).at(3) << " ";
    cout << atomic_coords.at(2).at(3) << " ";
    assert(atomic_coords.at(0).at(3) == 1.242623); 
    assert(atomic_coords.at(1).at(3) == -0.162327); 
    assert(atomic_coords.at(2).at(3) == 4.079396); 
    cout << "Atom 4 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(4) << " ";
    cout << atomic_coords.at(1).at(4) << " ";
    cout << atomic_coords.at(2).at(4) << " ";
    assert(atomic_coords.at(0).at(4) == -1.242623); 
    assert(atomic_coords.at(1).at(4) == 0.162327); 
    assert(atomic_coords.at(2).at(4) == 5.920604); 
    cout << "Atom 5 coordinates correct" << endl; 

    cout << atomic_coords.at(0).at(5) << " ";
    cout << atomic_coords.at(1).at(5) << " ";
    cout << atomic_coords.at(2).at(5) << " ";
    assert(atomic_coords.at(0).at(5) == -1.242623); 
    assert(atomic_coords.at(1).at(5) == -0.162327); 
    assert(atomic_coords.at(2).at(5) == 4.079396); 
    cout << "Atom 6 coordinates correct" << endl; 
 
  }

  cout << "Testing: LogReader read overlap" << endl;
  {

    string test_file = "testfile_overlap.log";
    {
      ofstream fid;
      fid.open(test_file);
      fid << " *** Overlap ***                                                             " << endl;
      fid << "                1             2             3             4             5" << endl;
      fid << "      1  0.100000D+01" << endl;
      fid << "      2  0.219059D+00  0.100000D+01" << endl;
      fid << "      3  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "      4  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "      5  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "      6  0.184261D+00  0.812273D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "      7  0.000000D+00  0.000000D+00  0.569754D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "      8  0.000000D+00  0.000000D+00  0.000000D+00  0.569754D+00  0.000000D+00" << endl;
      fid << "      9  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.569754D+00" << endl;
      fid << "     10  0.697718D-01  0.402091D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     11  0.000000D+00  0.000000D+00  0.164679D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     12  0.000000D+00  0.000000D+00  0.000000D+00  0.164679D+00  0.000000D+00" << endl;
      fid << "     13  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.164679D+00" << endl;
      fid << "     14  0.791027D-01  0.710235D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     15  0.791027D-01  0.710235D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     16  0.791027D-01  0.710235D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     17  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     18  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     19  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     20  0.481399D-05  0.194904D-01 -0.426713D-01  0.000000D+00  0.000000D+00" << endl;
      fid << "     21  0.194904D-01  0.203366D+00 -0.282219D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     22  0.426713D-01  0.282219D+00 -0.333594D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     23  0.000000D+00  0.000000D+00  0.000000D+00  0.115814D+00  0.000000D+00" << endl;
      fid << "     24  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.115814D+00" << endl;
      fid << "     25  0.643636D-01  0.359351D+00 -0.253511D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     26  0.129620D+00  0.562375D+00 -0.170740D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     27  0.000000D+00  0.000000D+00  0.000000D+00  0.242739D+00  0.000000D+00" << endl;
      fid << "     28  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.242739D+00" << endl;
      fid << "     29  0.527094D-01  0.309785D+00 -0.673317D-01  0.000000D+00  0.000000D+00" << endl;
      fid << "     30  0.555561D-01  0.303665D+00  0.598522D-01  0.000000D+00  0.000000D+00" << endl;
      fid << "     31  0.000000D+00  0.000000D+00  0.000000D+00  0.126532D+00  0.000000D+00" << endl;
      fid << "     32  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.126532D+00" << endl;
      fid << "     33  0.526767D-01  0.334402D+00 -0.413876D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     34  0.113280D-02  0.928256D-01 -0.142160D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     35  0.113280D-02  0.928256D-01 -0.142160D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     36  0.000000D+00  0.000000D+00  0.000000D+00  0.183392D+00  0.000000D+00" << endl;
      fid << "     37  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.183392D+00" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     39  0.320525D-01  0.270664D+00  0.190666D+00  0.543105D-01  0.308011D+00" << endl;
      fid << "     40  0.916644D-01  0.473962D+00  0.138280D+00  0.393885D-01  0.223384D+00" << endl;
      fid << "     41  0.320525D-01  0.270664D+00  0.190666D+00 -0.543105D-01 -0.308011D+00" << endl;
      fid << "     42  0.916644D-01  0.473962D+00  0.138280D+00 -0.393885D-01 -0.223384D+00" << endl;
      fid << "     43  0.430533D-04  0.789567D-02 -0.152645D-01  0.129366D-02  0.733675D-02" << endl;
      fid << "     44  0.142782D-01  0.109874D+00 -0.102097D+00  0.865264D-02  0.490716D-01" << endl;
      fid << "     45  0.430533D-04  0.789567D-02 -0.152645D-01 -0.129366D-02 -0.733675D-02" << endl;
      fid << "     46  0.142782D-01  0.109874D+00 -0.102097D+00 -0.865264D-02 -0.490716D-01" << endl;
      fid << "                6             7             8             9            10" << endl;
      fid << "      6  0.100000D+01" << endl;
      fid << "      7  0.000000D+00  0.100000D+01" << endl;
      fid << "      8  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "      9  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     10  0.727666D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     11  0.000000D+00  0.588690D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     12  0.000000D+00  0.000000D+00  0.588690D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     13  0.000000D+00  0.000000D+00  0.000000D+00  0.588690D+00  0.000000D+00" << endl;
      fid << "     14  0.629936D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.323540D+00" << endl;
      fid << "     15  0.629936D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.323540D+00" << endl;
      fid << "     16  0.629936D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.323540D+00" << endl;
      fid << "     17  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     18  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     19  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     20  0.643636D-01 -0.129620D+00  0.000000D+00  0.000000D+00  0.527094D-01" << endl;
      fid << "     21  0.359351D+00 -0.562375D+00  0.000000D+00  0.000000D+00  0.309785D+00" << endl;
      fid << "     22  0.253511D+00 -0.170740D+00  0.000000D+00  0.000000D+00  0.673317D-01" << endl;
      fid << "     23  0.000000D+00  0.000000D+00  0.242739D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     24  0.000000D+00  0.000000D+00  0.000000D+00  0.242739D+00  0.000000D+00" << endl;
      fid << "     25  0.579631D+00 -0.605354D+00  0.000000D+00  0.000000D+00  0.581168D+00" << endl;
      fid << "     26  0.605354D+00 -0.525884D-01  0.000000D+00  0.000000D+00  0.250193D+00" << endl;
      fid << "     27  0.000000D+00  0.000000D+00  0.579631D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     28  0.000000D+00  0.000000D+00  0.000000D+00  0.579631D+00  0.000000D+00" << endl;
      fid << "     29  0.581168D+00 -0.250193D+00  0.000000D+00  0.000000D+00  0.867984D+00" << endl;
      fid << "     30  0.491037D+00  0.258780D+00  0.000000D+00  0.000000D+00  0.461882D+00" << endl;
      fid << "     31  0.000000D+00  0.000000D+00  0.470171D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     32  0.000000D+00  0.000000D+00  0.000000D+00  0.470171D+00  0.000000D+00" << endl;
      fid << "     33  0.353148D+00 -0.422973D+00  0.000000D+00  0.000000D+00  0.254635D+00" << endl;
      fid << "     34  0.255918D+00 -0.441451D+00  0.000000D+00  0.000000D+00  0.247363D+00" << endl;
      fid << "     35  0.255918D+00 -0.441451D+00  0.000000D+00  0.000000D+00  0.247363D+00" << endl;
      fid << "     36  0.000000D+00  0.000000D+00  0.161252D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     37  0.000000D+00  0.000000D+00  0.000000D+00  0.161252D+00  0.000000D+00" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     39  0.375116D+00  0.267879D+00  0.763046D-01  0.432746D+00  0.258482D+00" << endl;
      fid << "     40  0.702351D+00  0.303675D+00  0.865010D-01  0.490572D+00  0.640357D+00" << endl;
      fid << "     41  0.375116D+00  0.267879D+00 -0.763046D-01 -0.432746D+00  0.258482D+00" << endl;
      fid << "     42  0.702351D+00  0.303675D+00 -0.865010D-01 -0.490572D+00  0.640357D+00" << endl;
      fid << "     43  0.740737D-01 -0.176934D+00  0.149950D-01  0.850413D-01  0.158045D+00" << endl;
      fid << "     44  0.262382D+00 -0.381297D+00  0.323148D-01  0.183267D+00  0.424412D+00" << endl;
      fid << "     45  0.740737D-01 -0.176934D+00 -0.149950D-01 -0.850413D-01  0.158045D+00" << endl;
      fid << "     46  0.262382D+00 -0.381297D+00 -0.323148D-01 -0.183267D+00  0.424412D+00" << endl;
      fid << "               11            12            13            14            15" << endl;
      fid << "     11  0.100000D+01" << endl;
      fid << "     12  0.000000D+00  0.100000D+01" << endl;
      fid << "     13  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     14  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     15  0.000000D+00  0.000000D+00  0.000000D+00  0.333333D+00  0.100000D+01" << endl;
      fid << "     16  0.000000D+00  0.000000D+00  0.000000D+00  0.333333D+00  0.333333D+00" << endl;
      fid << "     17  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     18  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     19  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     20 -0.555561D-01  0.000000D+00  0.000000D+00  0.526767D-01  0.113280D-02" << endl;
      fid << "     21 -0.303665D+00  0.000000D+00  0.000000D+00  0.334402D+00  0.928256D-01" << endl;
      fid << "     22  0.598522D-01  0.000000D+00  0.000000D+00  0.413876D+00  0.142160D+00" << endl;
      fid << "     23  0.000000D+00  0.126532D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     24  0.000000D+00  0.000000D+00  0.126532D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     25 -0.491037D+00  0.000000D+00  0.000000D+00  0.353148D+00  0.255918D+00" << endl;
      fid << "     26  0.258780D+00  0.000000D+00  0.000000D+00  0.422973D+00  0.441451D+00" << endl;
      fid << "     27  0.000000D+00  0.470171D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     28  0.000000D+00  0.000000D+00  0.470171D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     29 -0.461882D+00  0.000000D+00  0.000000D+00  0.254635D+00  0.247363D+00" << endl;
      fid << "     30  0.622202D+00  0.000000D+00  0.000000D+00  0.229601D+00  0.249594D+00" << endl;
      fid << "     31  0.000000D+00  0.867984D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     32  0.000000D+00  0.000000D+00  0.867984D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     33 -0.229601D+00  0.000000D+00  0.000000D+00  0.487216D+00  0.154963D+00" << endl;
      fid << "     34 -0.249594D+00  0.000000D+00  0.000000D+00  0.154963D+00  0.753228D-01" << endl;
      fid << "     35 -0.249594D+00  0.000000D+00  0.000000D+00  0.154963D+00  0.251076D-01" << endl;
      fid << "     36  0.000000D+00  0.236690D-01  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     37  0.000000D+00  0.000000D+00  0.236690D-01  0.000000D+00  0.000000D+00" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     39  0.109596D+00  0.312180D-01  0.177046D+00  0.207130D+00  0.123414D+00" << endl;
      fid << "     40  0.226998D+00  0.646599D-01  0.366705D+00  0.371459D+00  0.351303D+00" << endl;
      fid << "     41  0.109596D+00 -0.312180D-01 -0.177046D+00  0.207130D+00  0.123414D+00" << endl;
      fid << "     42  0.226998D+00 -0.646599D-01 -0.366705D+00  0.371459D+00  0.351303D+00" << endl;
      fid << "     43 -0.225200D+00  0.190856D-01  0.108240D+00  0.137730D-01  0.170528D-02" << endl;
      fid << "     44 -0.505665D+00  0.428549D-01  0.243043D+00  0.120267D+00  0.707321D-01" << endl;
      fid << "     45 -0.225200D+00 -0.190856D-01 -0.108240D+00  0.137730D-01  0.170528D-02" << endl;
      fid << "     46 -0.505665D+00 -0.428549D-01 -0.243043D+00  0.120267D+00  0.707321D-01" << endl;
      fid << "               16            17            18            19            20" << endl;
      fid << "     16  0.100000D+01" << endl;
      fid << "     17  0.000000D+00  0.100000D+01" << endl;
      fid << "     18  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     19  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     20  0.113280D-02  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     21  0.928256D-01  0.000000D+00  0.000000D+00  0.000000D+00  0.219059D+00" << endl;
      fid << "     22  0.142160D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     23  0.000000D+00 -0.183392D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     24  0.000000D+00  0.000000D+00 -0.183392D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     25  0.255918D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.184261D+00" << endl;
      fid << "     26  0.441451D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     27  0.000000D+00 -0.161252D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     28  0.000000D+00  0.000000D+00 -0.161252D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     29  0.247363D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.697718D-01" << endl;
      fid << "     30  0.249594D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     31  0.000000D+00 -0.236690D-01  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     32  0.000000D+00  0.000000D+00 -0.236690D-01  0.000000D+00  0.000000D+00" << endl;
      fid << "     33  0.154963D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.791027D-01" << endl;
      fid << "     34  0.251076D-01  0.000000D+00  0.000000D+00  0.000000D+00  0.791027D-01" << endl;
      fid << "     35  0.753228D-01  0.000000D+00  0.000000D+00  0.000000D+00  0.791027D-01" << endl;
      fid << "     36  0.000000D+00 -0.314243D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     37  0.000000D+00  0.000000D+00 -0.314243D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.000000D+00  0.753228D-01  0.000000D+00" << endl;
      fid << "     39  0.353785D+00  0.449500D-01  0.254925D+00  0.726146D-01  0.430533D-04" << endl;
      fid << "     40  0.406770D+00  0.108227D-01  0.613785D-01  0.174835D-01  0.142782D-01" << endl;
      fid << "     41  0.353785D+00 -0.449500D-01 -0.254925D+00  0.726146D-01  0.430533D-04" << endl;
      fid << "     42  0.406770D+00 -0.108227D-01 -0.613785D-01  0.174835D-01  0.142782D-01" << endl;
      fid << "     43  0.442596D-02 -0.178424D-02 -0.101190D-01  0.857578D-03  0.320525D-01" << endl;
      fid << "     44  0.818999D-01 -0.732391D-02 -0.415360D-01  0.352016D-02  0.916644D-01" << endl;
      fid << "     45  0.442596D-02  0.178424D-02  0.101190D-01  0.857578D-03  0.320525D-01" << endl;
      fid << "     46  0.818999D-01  0.732391D-02  0.415360D-01  0.352016D-02  0.916644D-01" << endl;
      fid << "               21            22            23            24            25" << endl;
      fid << "     21  0.100000D+01" << endl;
      fid << "     22  0.000000D+00  0.100000D+01" << endl;
      fid << "     23  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     24  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     25  0.812273D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     26  0.000000D+00  0.569754D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     27  0.000000D+00  0.000000D+00  0.569754D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     28  0.000000D+00  0.000000D+00  0.000000D+00  0.569754D+00  0.000000D+00" << endl;
      fid << "     29  0.402091D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.727666D+00" << endl;
      fid << "     30  0.000000D+00  0.164679D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     31  0.000000D+00  0.000000D+00  0.164679D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     32  0.000000D+00  0.000000D+00  0.000000D+00  0.164679D+00  0.000000D+00" << endl;
      fid << "     33  0.710235D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.629936D+00" << endl;
      fid << "     34  0.710235D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.629936D+00" << endl;
      fid << "     35  0.710235D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.629936D+00" << endl;
      fid << "     36  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     37  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     39  0.789567D-02  0.152645D-01  0.129366D-02  0.733675D-02  0.740737D-01" << endl;
      fid << "     40  0.109874D+00  0.102097D+00  0.865264D-02  0.490716D-01  0.262382D+00" << endl;
      fid << "     41  0.789567D-02  0.152645D-01 -0.129366D-02 -0.733675D-02  0.740737D-01" << endl;
      fid << "     42  0.109874D+00  0.102097D+00 -0.865264D-02 -0.490716D-01  0.262382D+00" << endl;
      fid << "     43  0.270664D+00 -0.190666D+00  0.543105D-01  0.308011D+00  0.375116D+00" << endl;
      fid << "     44  0.473962D+00 -0.138280D+00  0.393885D-01  0.223384D+00  0.702351D+00" << endl;
      fid << "     45  0.270664D+00 -0.190666D+00 -0.543105D-01 -0.308011D+00  0.375116D+00" << endl;
      fid << "     46  0.473962D+00 -0.138280D+00 -0.393885D-01 -0.223384D+00  0.702351D+00" << endl;
      fid << "               26            27            28            29            30" << endl;
      fid << "     26  0.100000D+01" << endl;
      fid << "     27  0.000000D+00  0.100000D+01" << endl;
      fid << "     28  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     29  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     30  0.588690D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     31  0.000000D+00  0.588690D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     32  0.000000D+00  0.000000D+00  0.588690D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     33  0.000000D+00  0.000000D+00  0.000000D+00  0.323540D+00  0.000000D+00" << endl;
      fid << "     34  0.000000D+00  0.000000D+00  0.000000D+00  0.323540D+00  0.000000D+00" << endl;
      fid << "     35  0.000000D+00  0.000000D+00  0.000000D+00  0.323540D+00  0.000000D+00" << endl;
      fid << "     36  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     37  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     39  0.176934D+00  0.149950D-01  0.850413D-01  0.158045D+00  0.225200D+00" << endl;
      fid << "     40  0.381297D+00  0.323148D-01  0.183267D+00  0.424412D+00  0.505665D+00" << endl;
      fid << "     41  0.176934D+00 -0.149950D-01 -0.850413D-01  0.158045D+00  0.225200D+00" << endl;
      fid << "     42  0.381297D+00 -0.323148D-01 -0.183267D+00  0.424412D+00  0.505665D+00" << endl;
      fid << "     43 -0.267879D+00  0.763046D-01  0.432746D+00  0.258482D+00 -0.109596D+00" << endl;
      fid << "     44 -0.303675D+00  0.865010D-01  0.490572D+00  0.640357D+00 -0.226998D+00" << endl;
      fid << "     45 -0.267879D+00 -0.763046D-01 -0.432746D+00  0.258482D+00 -0.109596D+00" << endl;
      fid << "     46 -0.303675D+00 -0.865010D-01 -0.490572D+00  0.640357D+00 -0.226998D+00" << endl;
      fid << "               31            32            33            34            35" << endl;
      fid << "     31  0.100000D+01" << endl;
      fid << "     32  0.000000D+00  0.100000D+01" << endl;
      fid << "     33  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     34  0.000000D+00  0.000000D+00  0.333333D+00  0.100000D+01" << endl;
      fid << "     35  0.000000D+00  0.000000D+00  0.333333D+00  0.333333D+00  0.100000D+01" << endl;
      fid << "     36  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     37  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00  0.000000D+00" << endl;
      fid << "     39  0.190856D-01  0.108240D+00  0.137730D-01  0.170528D-02  0.442596D-02" << endl;
      fid << "     40  0.428549D-01  0.243043D+00  0.120267D+00  0.707321D-01  0.818999D-01" << endl;
      fid << "     41 -0.190856D-01 -0.108240D+00  0.137730D-01  0.170528D-02  0.442596D-02" << endl;
      fid << "     42 -0.428549D-01 -0.243043D+00  0.120267D+00  0.707321D-01  0.818999D-01" << endl;
      fid << "     43  0.312180D-01  0.177046D+00  0.207130D+00  0.123414D+00  0.353785D+00" << endl;
      fid << "     44  0.646599D-01  0.366705D+00  0.371459D+00  0.351303D+00  0.406770D+00" << endl;
      fid << "     45 -0.312180D-01 -0.177046D+00  0.207130D+00  0.123414D+00  0.353785D+00" << endl;
      fid << "     46 -0.646599D-01 -0.366705D+00  0.371459D+00  0.351303D+00  0.406770D+00" << endl;
      fid << "               36            37            38            39            40" << endl;
      fid << "     36  0.100000D+01" << endl;
      fid << "     37  0.000000D+00  0.100000D+01" << endl;
      fid << "     38  0.000000D+00  0.000000D+00  0.100000D+01" << endl;
      fid << "     39  0.178424D-02  0.101190D-01  0.857578D-03  0.100000D+01" << endl;
      fid << "     40  0.732391D-02  0.415360D-01  0.352016D-02  0.658292D+00  0.100000D+01" << endl;
      fid << "     41 -0.178424D-02 -0.101190D-01  0.857578D-03  0.125798D-01  0.128011D+00" << endl;
      fid << "     42 -0.732391D-02 -0.415360D-01  0.352016D-02  0.128011D+00  0.365470D+00" << endl;
      fid << "     43 -0.449500D-01 -0.254925D+00  0.726146D-01  0.571644D-03  0.366357D-01" << endl;
      fid << "     44 -0.108227D-01 -0.613785D-01  0.174835D-01  0.366357D-01  0.168873D+00" << endl;
      fid << "     45  0.449500D-01  0.254925D+00  0.726146D-01  0.104764D-04  0.720595D-02" << endl;
      fid << "     46  0.108227D-01  0.613785D-01  0.174835D-01  0.720595D-02  0.617178D-01" << endl;
      fid << "               41            42            43            44            45" << endl;
      fid << "     41  0.100000D+01" << endl;
      fid << "     42  0.658292D+00  0.100000D+01" << endl;
      fid << "     43  0.104764D-04  0.720595D-02  0.100000D+01" << endl;
      fid << "     44  0.720595D-02  0.617178D-01  0.658292D+00  0.100000D+01" << endl;
      fid << "     45  0.571644D-03  0.366357D-01  0.125798D-01  0.128011D+00  0.100000D+01" << endl;
      fid << "     46  0.366357D-01  0.168873D+00  0.128011D+00  0.365470D+00  0.658292D+00" << endl;
      fid << "               46" << endl;
      fid << "     46  0.100000D+01" << endl;
      fid.close();
    }

    LogReader lr(test_file);
    lr.read();

    Eigen::MatrixXd overlap = lr.getOverlapMatrix(); 

    cout << "Overlap Matrix rows " << overlap.rows() << " cols " << overlap.cols() << endl;
    assert(overlap.rows()==46);
    assert(overlap.cols()==46);
    // We will test the corners of the overlap matrix and a few elements 
    // at random
    cout << "row  1  1 1.000 " << overlap(0,0) << endl;
    assert(overlap(0,0)==1);
    cout << "row  1 46 0.01427 " << overlap(0,45) << endl;
    assert(overlap(0,45)==0.0142782);
    cout << "row 46  1 0.01427 " << overlap(45,0) << endl;
    assert(overlap(45,0)==0.0142782);
    cout << "row 46 46 1.000 " << overlap(45,45) << endl;
    assert(overlap(45,45)==1);

    assert(overlap(10,6)==0.588690);
    assert(overlap(6,10)==0.588690);

    assert(overlap(38,27)==0.0850413);
    assert(overlap(27,38)==0.0850413);

    assert(overlap(26,22)==0.569754);
    assert(overlap(26,22)==0.569754);
  }

  cout << "Testing: LogReader read" << endl;
  {

    LogReader lr("../../../GAUSSIANFILES/90_unordered/90_pair.log");
    lr.read();
    auto orb_info = lr.getOrbitalInfo();

    auto Soverlap = lr.getOverlapMatrix();
    cout << Soverlap.rows() << endl;
    auto Alpha = lr.getOE("Alpha");

    auto basisFuncCount = lr.getBasisFuncCount();
    for (auto c : basisFuncCount) {
      cout << c << endl;
    }

    vector<vector<double>> xyz = lr.getCoords();
    auto x = xyz.at(0);
    auto y = xyz.at(1);
    auto z = xyz.at(2);
    for (size_t ind = 0; ind < x.size(); ++ind) {
      cout << x.at(ind) << " " << y.at(ind) << " " << z.at(ind) << endl;
    }
  }
  return 0;
}

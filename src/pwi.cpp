/* $Id: pwi.cc,v 1.7 2004/05/06 07:30:26 twogood Exp $ */
#include "pwidocument.h"
#include "binarypwiparser.h"
#include "abstractpwiexporter.h"
#include "xhtmlpwiexporter.h"
#include "configuration.h"
#include "openofficepwiexporter.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#define VERBOSE 1

using namespace std;
using namespace pwi;


#if 0
static void dump(const char *desc, void* data, size_t len)/*{{{*/
{
  uint8_t* buf = (uint8_t*)data;
  size_t i, j;
  char hex[8 * 3 + 1];
  char chr[8 + 1];

  fprintf(stderr, "%s (%d bytes):\n", desc, len);
  for (i = 0; i < len + 7; i += 8) {
    for (j = 0; j < 8; j++) 
      if (j + i >= len) {
        hex[3*j+0] = ' ';
        hex[3*j+1] = ' ';
        hex[3*j+2] = ' ';
        chr[j] = ' ';
      } else {
        uint8_t c = buf[j + i];
        const char *hexchr = "0123456789abcdef";
        hex[3*j+0] = hexchr[(c >> 4) & 0xf];
        hex[3*j+1] = hexchr[c & 0xf];
        hex[3*j+2] = ' ';
        if (c > ' ' && c <= '~')
          chr[j] = c;
        else
          chr[j] = '.';
      }
    hex[8*3] = '\0';
    chr[8] = '\0';
    fprintf(stderr, "  %04x: %s %s\n", i, hex, chr);
  }
}/*}}}*/
#endif


#include "configuration.h"

/**
 * show some text when user says -h
 */
void displayHelp() {
    cout << "pwi2html [options] [filename]" << endl;
    cout << "options:" << endl;
    cout << "-h help " << endl;
    cout << "-o output file name " << endl;
    cout << "-f output format (text | xhtml | sxw) " << endl;
}

/**
 * display what we understood from the command line (the contents of the Configuration object)
 */
void displayParsedOptions(Configuration& conf) {
    cerr << "Running with the following options: " << endl;
    cerr << "show help: " << conf.SHOW_HELP  << endl;
    cerr << "input filename: " << conf.IN_FILENAME  << endl;
    cerr << "output filename: " << conf.OUT_FILENAME  << endl;
    cerr << "output format: " << conf.OUT_FORMAT << endl;
}


void saveResult(istream& exportedContent, Configuration& conf) {
    string tmp;

    ostream* out;
    bool dontdelete = false;
    
    if (conf.OUT_FILENAME.size() == 0) // no out file was specifed, use console
    {
        //out = new ofstream(cout);
        out = &cout;
        dontdelete = true;
    } else {
        out = new ofstream(conf.OUT_FILENAME.c_str(), ios::binary);
    }
    
    const int BUFFSIZE = 300;
    char buffer[BUFFSIZE];
    int bytesread;
    while (exportedContent.good()) {
        exportedContent.read(buffer, BUFFSIZE);
//         out->write(buffer, BUFFSIZE);
        out->write(buffer, exportedContent.gcount());
    }
    //out->write("[***]", 5);
//     out->write(buffer, exportedContent.gcount());
    out->flush();
    if (!dontdelete) delete out;
}

int main(int argc, char** argv) {
        try {

            Configuration conf;
            conf.parseCommandLine(argc, argv);
    
            //displayParsedOptions(conf);
    
            if (conf.SHOW_HELP) {
                displayHelp();
                exit (0);
            }
        
            istream* is;
            
            if (conf.IN_FILENAME.size() == 0) // use the standard input  
                is = &cin;
            else {
                is = new ifstream (conf.IN_FILENAME.c_str(), ifstream::binary);
                if (! ((ifstream*)is)->is_open())
                    throw runtime_error(string("cannot read from file '") + conf.IN_FILENAME + "'");
            }
    
            BinaryPwiParser parser (*is);
            PwiDocument doc;
            parser.parse(doc);
            
            AbstractPwiExporter* exporter;
            if (conf.OUT_FORMAT == Configuration::OUT_FORMAT_XHTML 
                || conf.OUT_FORMAT == Configuration::OUT_FORMAT_HTML)
                exporter = new XhtmlPwiExporter (doc);
            else
            if (conf.OUT_FORMAT == Configuration::OUT_FORMAT_TEXT)
                throw runtime_error(string("text export not yet implemented"));
            else
                exporter = new OpenofficePwiExporter(doc);

            
            //string exportedHtml;
            istream* exportedStream = exporter->exportPwi();
            
            if (exportedStream != NULL)
                saveResult(*exportedStream, conf);
            
    
            // finish things up
            // TODO: these should be in some kind of finally block otherwise there may be leaks on exceptions thrown
            if (exportedStream != NULL) delete exportedStream;
            if (exporter != NULL)       delete exporter;
            if (is != NULL)             delete is;
                
        } catch (runtime_error err) {
            cerr << "Runtime exception: " << err.what() << endl;
            abort();
        } catch (...) {
            cerr << "an exception occured."  << endl;
            abort();
        }
        

}


#include "client.h"
#include "searcher.h"

class Editer
{
    deque<double> edit_points;
    string src;
    double trim_front;
    double trim_back;
private:
    void exec_concat();
    string get_offset_as_string(double p);
    string get_duration_as_string();
public:
    Editer();
    Editer(string s, double front=5.0, double back=2.0);
    void set_edit_points(deque<double>);
    void exec_trimming();
    string get_input_filename();
};
Editer::Editer()
{
    // dummy
}
Editer::Editer(string s, double front, double back) : src(s), trim_front(front), trim_back(back)
{
    path ph{src};
    if(!exists(ph))
    {
        throw "Error:Input file not exist:" + ph.string();
    }
    cout << "INFO:OFFSET is " + to_string(-trim_front) + " ~ " + to_string(trim_back) + " sec." << endl; 
}
/*
    private function
*/
void Editer::exec_concat()
{
    const char *cmd = "ffmpeg -f concat -i edit_list.txt -c copy -y output/00_output.mp4";
    std::system(cmd);
}
string Editer::get_duration_as_string()
{
    stringstream ss;
    ss << " ";
    ss << trim_front + trim_back;
    ss << " ";
    return ss.str();
}
string Editer::get_offset_as_string(double p)
{
    if (p - trim_front < 0)
    {
        return " 0 ";
    }
    stringstream ss;
    ss << " ";
    ss << p - trim_front;
    ss << " ";
    return ss.str();
}
/*
    public function
*/
void Editer::set_edit_points(deque<double> p){
    edit_points = p;
}
void Editer::exec_trimming()
{
    deque<string> command_list;
    deque<string> file_list;
    int index = 1;

    if(edit_points.empty())
    {
        throw string("Error:Edit points is empty");
    }

    for(auto p : edit_points)
    {
        stringstream ss;
        ss << index++;
        string dst = "output/cut" + ss.str() + ".mp4";

        string cmd = "ffmpeg -ss " + get_offset_as_string(p) + " -i " + src + " -y -t " + get_duration_as_string() + dst;
        cout << cmd << endl;
        command_list.push_back(cmd);

        string file;
        file = "file " + dst;
        file_list.push_back(file);
    }

    ofstream fout;
    fout.open("edit_list.txt");
    for(auto f : file_list)
    {
        fout << f << endl;
    }
    fout.close();

    for(auto c : command_list)
    {
        const char* cmd_as_char = c.c_str();
        std::system(cmd_as_char);
    }

    exec_concat();

}
string Editer::get_input_filename()
{
    return src;
}
int main(int argc, char** argv)
{
    try
    {
        Editer e;
        if (argc == 2)
        {
            e = Editer(argv[1]);
        }
        else if(argc == 4)
        {
            e = Editer(argv[1], atof(argv[2]), atof(argv[3]));
        }
        else
        {
            throw string("Usage:client.exe [input video filename] [offset_front(default=5.0) offset_back(default=2.0)]");
        }
        
        
        deque<double> edit_points;
        search_edit_points(e.get_input_filename() ,&edit_points);
        e.set_edit_points(edit_points);
        e.exec_trimming();

        cout << "INFO:Edit Finished" << endl;
    }
    catch(string s)
    {
        cout << s << endl;
    }
    catch(exception &err)
    {
        cout << err.what() << endl;
    }
    catch(...)
    {
        cout << "Unknown Error" << endl;
    }

    cout << "[Press Enter...]";
    cin.ignore(1);

}
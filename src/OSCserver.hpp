#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <FL/Fl_Group.H>
#include <FL/Fl_Widget.H>


#include <lo/lo.h>


void error(int num, const char *msg, const char *path);
static int button_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
void add_method(const char *path, lo_server_thread st);


	

class OSCserver{
private:
public:
	lo_server_thread server;
	OSCserver(){
		server = lo_server_thread_new( "6666", error );
		std::cout << "Nouveau serveur OSC sur le port udp 6666" << std::endl;
		const char *path = "/pine/couille";
		add_method_wrapper(path);
//	        lo_server_thread_start(server);
//		server_start();
		std::cout << "Serveur en route" << std::endl;
	};

	void add_method_wrapper(const char *path){
//		lo_server_thread_stop(server);
		lo_server_thread_add_method(server, path, NULL, button_handler, NULL);
//		add_method(path, server);
//		lo_server_thread_start(server);
	};

	void server_start(){
		lo_server_thread_start(server);
	};
	

	~OSCserver(){};

	lo_server_thread server_(){ return server; };

    char   **my_explode(char *str, char separator)
    {
      char **res = NULL;
      int  nbstr = 1;
      int  len;
      int  from = 0;
      int  i;
      int  j;
   
     res = (char **) malloc(sizeof (char *));
     len = strlen(str);
     for (i = 0; i <= len; ++i)
       {
         if ((i == len) || (str[i] == separator))
      {
        res = (char **) realloc(res, ++nbstr * sizeof (char *));
        res[nbstr - 2] = (char *) malloc((i - from + 1) * sizeof (char));
        for (j = 0; j < (i - from); ++j)
            res[nbstr - 2][j] = str[j + from];
        res[nbstr - 2][i - from] = '\0';
        from = i + 1;
        ++i;
      }
       }
     res[nbstr - 1] =  NULL;
     return res;
   }

};

void error(int num, const char *msg, const char *path)
{
	std::cout << "liblo server error " << num << " in path " << path << ": " << msg << std::endl;
}

static int button_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	std::cout << "Alors ?" << std::endl;
};
void add_method(const char *path, lo_server_thread st){
		lo_server_thread_add_method(st, path, NULL, button_handler, NULL);
};

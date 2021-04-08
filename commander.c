#include "commander.h"
#include "string_macros.h"

void reset_cmd(){
    memset(command_, '\0', sizeof(command_));
    bytes_ = 0;
    is_param_ = 0;
    param_ = 0;
}

void add_callback(char* command, void (* callback)(unsigned char param)){
    strcpy(commands_[cb_num_], command);
    callbacks_[cb_num_] = callback;
    cb_num_++;
}

void process_char(unsigned char dat){
    bytes_++;
    
    if(bytes_ == COMMAND_LEN + 1){
        param_ = dat;
        is_param_ = 1;
    } else{
        command_[bytes_ - 1] = dat;
    }
    
    if(is_param_){
        // find the command and call its callback function
        for(unsigned char i = 0 ; i < cb_num_ ; i++){
            if(strings_eq(command_, commands_[i])){
                callbacks_[i](param_);
                break;
            }
        }
        
        reset_cmd();
    }
}
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;
//ALU select of 2 bits for ease
//design decision to get rid of ALU op and give opcode to ALUcontrol
//choice of int for its intrinsic 4 bytes for register
//string to integer immediate
int stii(string bin){
//string bin="100000000000";
char signbit=bin[0];
string mag=bin.substr(1);
int imm;
if(signbit=='1'){
    //7FF is the 11 bit all 1 integer
    int complement_mag=stoi(mag,nullptr,2);
    int complement_1s=complement_mag^0x7FF;
    int complement_2s=complement_1s+1;
    imm=-complement_2s;
    //cout<<imm<<endl;
}
else{
    int complement_mag=stoi(mag,nullptr,2);
    imm=complement_mag;
}
    return imm;
}

//string to register
int stir(string reg){
    return stoi(reg,nullptr,2);
}



class controller{
    public:
    map<string,bool> cw_map;
    controller(string opcode){
        //first 3 bits of opcode
        bool op0=stoi(opcode.substr(0,1));
        bool op1=stoi(opcode.substr(1,1));
        bool op2=stoi(opcode.substr(2,1));//opcode_6,5,4 respectively
        cw_map["ALUsrc"]=!op1||!op0&&op1&&!op2;//ok
        cw_map["memreg"]=!op0&&!op1&&!op2;
        cw_map["regwrite"]=!op1||op2;
        cw_map["memwrite"]=!op0&&op1&&!op2;
        cw_map["memread"]=!op0&&!op1&&!op2;
        cw_map["branch"]=op0&&op1;
        // cw_map["aluop1"]=op2;
        // cw_map["aluop2"]=op0&&op1;
        cw_map["regread"]=1;
        //only the things completely dermined from opcode

        //for testing
        //cout<<cw_map["ALUsrc"]<<" "<<cw_map["mem_Reg"]<<" "<<cw_map["reg_write"];
    }
    controller(){

    }
};


string ALU_control(string func3,string func7,string opcode){
    //getiing the relevent bits
    bool func7_30=stoi(func7.substr(1,1));//the 30th bit
    bool func3_0=stoi(func3.substr(0,1));
    bool func3_1=stoi(func3.substr(1,1));
    bool func3_2=stoi(func3.substr(2,1));//all the func3 bits
    bool op0=stoi(opcode.substr(0,1));//useless same for all required
    bool op1=stoi(opcode.substr(1,1));
    bool op2=stoi(opcode.substr(2,1));

    int ALUC_1=func3_0;
    int ALUC_2=(func7_30||func3_2)&&op1&&op2;
    return to_string(ALUC_1)+to_string(ALUC_2);
    
}
class Forwarder{
public:
//no state
//get the data dependency in decode stage and check for the required values in MEM and Execute stage
bool cant_resolve(int rs);
void resolve();
}forwarder;
class DM{
    public:
    map<int,int> DataMem;
    DM(){
        ifstream dat_file("DM.csv");
        string dat_line;
        while(getline(dat_file,dat_line)){
            int comma=dat_line.find(',');
            DataMem[stoi(dat_line.substr(0,comma))]=stoi(dat_line.substr(comma+1));
        }
    }
        void write(int address,int data){
        ofstream dat_file1("DM.csv",ios::app);
        dat_file1<<to_string(address)+","+to_string(data)<<endl;
    }
}dm;
//DM
map<int,string> IM;
//IM
int GPR[32];
int ins[32];
//GPR
int Prev_val_of_ins_rdl;

class ALU{
    public:
    int compute(int ip1,int ip2,string ALU_sel){
        int select=stoi(ALU_sel,nullptr,2);
        //multiplexing on select
        switch (select){
            case 0:return ip1+ip2;
            case 1:return ip1-ip2;  
            case 2:return ip1|ip2;
            case 3:return ip1&ip2;
        }
    }
}alu;

//the pipeline register set
struct PC_register_set{
    public:
    int pc;
    bool valid;
    bool stall;
}PC;
struct IFID_register_set{
    public:
    string IR;
    int DPC;
    bool valid;
    bool stall;
}ifid;

typedef struct func_bits{
    string opcode;
    string func3;
    string func7;  

}func;

struct  IDEX_register_set{
    int DPC;
    string instruction;
    int immf;
    controller cw;
    int rs1;
    int rs2;
    func func_b;
    int rdl;
    bool valid;
    bool stall;
    int rsl1;
    int rsl2;
}idex;


struct EXMO_register_set{
    int DPC;
    controller cw;
    string instruction;
    int ALU_out;
    int rs2;
    int rdl;
    bool valid;
    bool stall;
    bool is_branch_taken;
}exmo;

struct MOWB_register_set{
    int DPC;
    string instruction;
    controller cw;
    int ld_out;
    int alu_out;
    int rdl;
    bool valid;
    bool stall;
}mowb;
// global NPC
int NPC;
int TPC;
string s(31,'0');
bool Forwarder::cant_resolve(int rs){
//see the dependency in decode stage(the same detection system may work)  
//can't resolve when the value is from a load instruction in execute stage
if(rs==idex.rdl && idex.cw.cw_map["regwrite"] && idex.cw.cw_map["memread"]){
    return true;
}
return false;
//whatever is in the execute stage if it is load and it writes into 
//that register then the forwarder can't resolve
}
void Forwarder::resolve(){
    //detect and resolve any dependency
    //forwarding from exmo register set
    
    bool rs1_forwarded_by_exmo=false;
    bool rs2_forwarded_by_exmo=false;
    if(exmo.valid && idex.valid){
    if(exmo.rdl==idex.rsl1 && exmo.rdl!=0&& exmo.cw.cw_map["regwrite"] && !exmo.cw.cw_map["memread"]){
    //if the registers match and the source wants to write and the output is available right now
            idex.rs1=exmo.ALU_out;
            rs1_forwarded_by_exmo=true;
        }
    if(exmo.rdl==idex.rsl2 && exmo.rdl!=0&& exmo.cw.cw_map["regwrite"] &&!exmo.cw.cw_map["memread"] && !idex.cw.cw_map["ALUsrc"]) {
    //rest same forward just for branch and R-R operation
        idex.rs2=exmo.rs2;
        rs2_forwarded_by_exmo=true;
    }
    }
    if(mowb.valid && idex.valid){
    if(mowb.rdl==idex.rsl1 && mowb.rdl!=0 && mowb.cw.cw_map["regwrite"] && !rs1_forwarded_by_exmo){
        if(mowb.cw.cw_map["memreg"]){
            idex.rs1=mowb.ld_out;
        }
        else{
            idex.rs1=mowb.alu_out;
        }
    }
    if(mowb.rdl==idex.rsl2 && mowb.rdl!=0 && mowb.cw.cw_map["regwrite"] && !idex.cw.cw_map["ALUsrc"] && !rs2_forwarded_by_exmo){
        if(mowb.cw.cw_map["memreg"]){
            idex.rs2=mowb.ld_out;
        }
        else{
            idex.rs2=mowb.alu_out;
        }
    }
    }
}


void fetch(){
if (!PC.valid || ifid.stall){return;}
    string instruction=IM[PC.pc];

    if(instruction==s){PC.valid=false;}
    ifid.IR=instruction;
    ifid.DPC=PC.pc;
    
    ifid.valid=true;
    return;
}
void add_bubble_in_execute(){
    //for bubble:
    string nop="00000000000100000000000000010011";
    string opcode=nop.substr(25,7);
    string func3=nop.substr(17,3);
    string func7=nop.substr(0,7);
    controller cw(opcode);
    idex.cw=cw;
    idex.func_b.func3=func3;
    idex.func_b.func7=func7;
    idex.func_b.opcode=opcode;
    idex.instruction=nop;
    idex.rdl=0;
    idex.DPC=-1;
}
void add_bubble_in_decode(){
    //for bubble:
    string nop="00000000000100000000000000010011";
    ifid.DPC=-1;
    ifid.stall=false;
    ifid.IR=nop;
    ifid.valid=true;
}
void decode(){
    if(!ifid.valid ||idex.stall){return;}
    string instruction=ifid.IR;
    if(instruction==s){ifid.valid=false;}
    int PC_decode=ifid.DPC;
    //immediate generator
    string immil=instruction.substr(0,12);//imm for immediate & load 
    string imms=instruction.substr(0,7)+instruction.substr(20,5);
    string immb=instruction.substr(0,1)+instruction.substr(24,1)+instruction.substr(1,6)+instruction.substr(20,4);
    //

    int rsl1=stir(instruction.substr(12,5));
    int rsl2=stir(instruction.substr(7,5));
    int rd=stir(instruction.substr(20,5));
    string opcode=instruction.substr(25,7);

    //making control word
    controller cw(opcode);
    
    //register read
    //decide what type of instruction it is and put the locks accordingly

    int rs1,rs2;
    if(cw.cw_map["regread"] ){
        if(ins[rsl1]==-1){
            rs1=GPR[rsl1];    
        }
        else if(forwarder.cant_resolve(rsl1)){
            ifid.stall=true;
            add_bubble_in_execute();
            return;
        }
        //if forwarder can resolve move ahead
    }
      
    if(cw.cw_map["regread"] &&  (opcode=="0110011"|| opcode=="0100011"||opcode=="1100011")){//checks is R type or store or branch
        if(ins[rsl2]==-1){
            rs2=GPR[rsl2];  
        }
    else if(forwarder.cant_resolve(rsl1)){
            ifid.stall=true;
            add_bubble_in_execute();
            return;
        }
    }

    if(cw.cw_map["regwrite"] && rd!=0){
        //but if the next instruction is a brach this may get flushed --> restore to previous value in that case
        Prev_val_of_ins_rdl=ins[rd];
        //if writes into the rd register 
        ins[rd]=PC_decode;//then lock the register
    }
    
    if(cw.cw_map["memwrite"]){
        idex.immf=stii(imms);
    }
    else if(cw.cw_map["branch"]){
        idex.immf=stii(immb);
    }
    else{
        idex.immf=stii(immil);
    }

    string func3=instruction.substr(17,3);
    string func7=instruction.substr(0,7);

    idex.instruction=instruction;
    idex.DPC=PC_decode;
    idex.cw=cw;
    idex.rs1=rs1;
    idex.rs2=rs2;
    idex.func_b.func3=func3;
    idex.func_b.func7=func7;
    idex.func_b.opcode=opcode;
    idex.rdl=rd;
    idex.rsl1=rsl1;
    idex.rsl2=rsl2;
    idex.valid=true;
    ifid.stall=false;
    return;
}
void execute(){
    if(!idex.valid||exmo.stall){return;}
    string instruction=idex.instruction;
    if(instruction==s){idex.valid=false;}
    int PC_execute=idex.DPC;
    int immf=idex.immf;
    controller cw=idex.cw;
    int rs1=idex.rs1;
    int rs2=idex.rs2;
    func_bits func_b=idex.func_b;
    string func3=func_b.func3;
    string func7=func_b.func7;
    string opcode=func_b.opcode;
    int rd=idex.rdl;

    //generating ALU control
    string aluselect=ALU_control(func3,func7,opcode);
    //

    //for execution
    int input_data1=rs1;
    int input_data2;
    if(cw.cw_map["ALUsrc"]){
        input_data2=immf;
    }else{
    input_data2=rs2;
    }
     
    int ALU_result=alu.compute(input_data1,input_data2,aluselect);
    bool zero_flag=(rs1==rs2);
    bool lt_flag=(rs1<rs2);
    bool gt_flag=(rs1>rs2);
   

    int BPC= PC_execute+immf*4;
    // int TPC;
    if(cw.cw_map["branch"]){
        switch (stir(func3)){
            case 0://beq
            if(zero_flag){TPC=BPC;
            exmo.is_branch_taken=true;
            }
            else{TPC=NPC;
            exmo.is_branch_taken=false;
            }
            break;
            case 5:
            if(zero_flag||gt_flag){TPC=BPC;
            exmo.is_branch_taken=true;
            }
            else{TPC=NPC;
            exmo.is_branch_taken=false;
            }
            break;
            case 7:
            if(lt_flag){TPC=BPC;
            exmo.is_branch_taken=true;
            }
            else{TPC=NPC;
            exmo.is_branch_taken=false;
            }
            break;
        }
    }else{
        TPC=NPC;
        exmo.is_branch_taken=false;
    }
    //PC.pc=TPC;//here the hazard logic and flushing the pipeline will come
exmo.DPC=idex.DPC;
exmo.instruction=instruction;
exmo.cw=cw;
exmo.ALU_out=ALU_result;
exmo.rdl=rd;
exmo.rs2=rs2;
exmo.valid=true;
return;
}
void memory(){
    if(!exmo.valid||mowb.stall){return;}
    string instruction=exmo.instruction;
    if(instruction==s){exmo.valid=false;}
    controller cw=exmo.cw;
    int ALU_result=exmo.ALU_out;
    int rd=exmo.rdl;
    int rs2=exmo.rs2;
    int ldresult;
   if(cw.cw_map["memread"]){
        ldresult=dm.DataMem[ALU_result];
    }
    if(cw.cw_map["memwrite"]){
        dm.write(ALU_result,rs2);
    }
mowb.DPC=exmo.DPC;
mowb.instruction=instruction;
mowb.alu_out=exmo.ALU_out;
mowb.cw=exmo.cw;
mowb.ld_out=ldresult;
mowb.rdl=rd;
mowb.valid=true;

    }
void wb(){
    if(!mowb.valid){return;}

    string instruction = mowb.instruction;
    if(instruction==s){mowb.valid=false;}
    int PC_writeback=mowb.DPC;
    int ALU_result=mowb.alu_out;
    int ldresult=mowb.ld_out;
    controller cw=mowb.cw;
    int rd=mowb.rdl;
    int write_res;
      if(cw.cw_map["regwrite"]){
        if(cw.cw_map["memreg"]){
            write_res=ldresult;
        }else{
             write_res=ALU_result;
        }
        if(ins[rd]==PC_writeback){
            GPR[rd]=write_res;
            ins[rd]=-1;//unlock
        }
    }
   
    }
void make_IM_GPR_INS(){
for(int i=0;i<32;i++){GPR[i]=0;ins[i]=-1;}
string instruction;
ifstream inputfile("machinecode.txt");
int PC=0;
while(getline(inputfile,instruction)){
IM[PC]=instruction;
PC=PC+4;
}
string s(31,'0');
s=s+"1";
IM[PC]=s;
//return PC_exit;
}

int main(){
    s=s+"1";
make_IM_GPR_INS();
//load the pipeline
PC.valid=true;
PC.pc=0;
int CC=0;
//int CC_after_branch_fetch=0;
while(PC.valid||ifid.valid||idex.valid||exmo.valid||mowb.valid){
    CC+=1;
    if (CC == 1) {
    cout << left << setw(6) << "cycle"<<setw(8) << "fetch"<<setw(8) << "decode"<<setw(8) << "execute"<<setw(8) << "memory"<< "writeback" << endl;
    }
    cout<<left<<setw(8)<<CC<<setw(8)<<PC.pc<<setw(8)<<ifid.DPC<<setw(8)<<idex.DPC<<setw(8)<<exmo.DPC<<mowb.DPC<<endl;
    //operant forwarding should also happen here
    NPC=PC.pc+4;
    forwarder.resolve();
    wb();
    memory();
    execute();
    decode();
    fetch();
    if(!ifid.stall){//if ifid stalls dont change the pc
    if(CC<3){
            PC.pc=NPC;
        }
        else{
            PC.pc=TPC;
        }
    }
    //if branch taken --> flush 
    if(exmo.cw.cw_map["branch"] && exmo.is_branch_taken){
        //ifid.flush and idex.flush
        ins[idex.rdl]=Prev_val_of_ins_rdl;
        add_bubble_in_decode();
        add_bubble_in_execute();
    }
}
}


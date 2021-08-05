#include<bits/stdc++.h>
using namespace std;
class Reservation_Station
{
public:
    /* data */
    bool isBusy;
    int Qj, Qk, Vj, Vk, lat, op, result, instNum, ISSUE_Lat, WRITEBACK_Lat;
    bool resultReady;

    Reservation_Station(){
        isBusy = false;
        op = 0;
        lat = 0;
        result = 0;
        resultReady = false;
        Qj = 1;
        Qk = 1;
        Vj = 0;
        Vk = 0;
        instNum = 100000;
        this->ISSUE_Lat = 0;
        WRITEBACK_Lat = 0;
    }
    Reservation_Station(int OP, int RSoperandAvail){
        isBusy = false;
        op = OP;
        lat = 0;
        result = 0;
        resultReady = false;
        Qj = RSoperandAvail;
        Qk = RSoperandAvail;
        Vj = 0;
        Vk = 0;
        instNum = 100000;
        ISSUE_Lat = 0;
        WRITEBACK_Lat = 0;
    }
    ~Reservation_Station();
};

Reservation_Station::~Reservation_Station()
{
}

class Instruction
{

public:
    int rd, rs, rt, op, issueClock, executeClockBegin, executeClockEnd, writebackClock, imm;
    Instruction(){
        rd = 0;
        rs = 0;
        rt = 0;
        op = 0;
        issueClock = 0;
        executeClockBegin = 0;
        executeClockEnd = 0;
        writebackClock = 0;
        imm=0;
    }
    Instruction(int RD, int RS, int RT, int OP, int imm=0){
        rd=RD;
        rs=RS;
        rt=RT;
        op=OP;
        issueClock=0;
        executeClockBegin = 0;
        executeClockEnd = 0;
        writebackClock = 0;
        this->imm=imm;
    }
    ~Instruction();
};

Instruction::~Instruction()
{
}


class RegisterStatus
    {
public:
    //bool busy;
    int Qi;
    RegisterStatus(){
        //busy=false;
        Qi=0;
    }
    RegisterStatus(int a){
        //busy=false;
        Qi=a;
    }

    ~RegisterStatus(    );
};
RegisterStatus::~RegisterStatus(    )
{
}

class MemoryStatus{
    public:
        //bool busy;
        int Qi;
        MemoryStatus(){
            //busy=false;
            Qi=0;
        }
        MemoryStatus(int add){
            //busy=false;
            Qi=add;
        }
        ~MemoryStatus(){}
};


const int Mem_size=64;
vector<int> Memory(Mem_size);
vector<MemoryStatus> MemStatus(Mem_size);


const int Num_ADD_RS=4;
const int Num_MUL_RS=3;
const int Num_DIV_RS=2;
const int Num_AND_RS=4;
const int Num_OR_RS=4;
const int Num_XOR_RS=3;
const int Num_NOT_RS=4;
const int Num_LSL_RS=4;
const int Num_LSR_RS=4;
const int Num_Load_RS=1;
const int Num_Store_RS=1;
const int Tot_RS=Num_ADD_RS+Num_MUL_RS+Num_DIV_RS+Num_AND_RS+Num_OR_RS+Num_XOR_RS+Num_NOT_RS+Num_LSL_RS+Num_LSR_RS+Num_Load_RS+Num_Store_RS;


const int AddOp=0;
const int SubOp=1;
const int MulOp=2;
const int DivOp=3;
const int ANDop=4;
const int OROp=5;
const int XOROp=6;
const int NOTOp=7;
const int LSLOp=8;
const int LSROp=9;
const int ModOp=10;
const int LoadOp=11;
const int StoreOp=12;

const int AddLat=4;
const int MulLat=8;
const int DivLat=10;
const int ANDLat=2;
const int ORLat=2;
const int XORLat=3;
const int NOTLat=1;
const int LSLLat=4;
const int LSRLat=4;
const int LoadLat=10;
const int StoreLat=10;

const int ISSUE_Lat=2;
const int WRITE_BACK_Lat=2;

const int Num_reg_count=13;     //F0 to F12, F0 set to ZERO_REG


int clock_=0;
bool Done=false;
bool INSTR_left=true;
int Total_Instr=0, Tot_Write_Back=0;
int currINST_ISSUE=1;
const int ZERO_REG=0;
const int RegStatusEmpty=1000;
const int OperandAvail=1001;
const int OperantINIT=1002;
const int MemoryStatusEmpty=1003;
int ADD_beg, ADD_end;
int MUL_beg, MUL_end;
int DIV_beg, DIV_end;
int AND_beg, AND_end;
int OR_beg, OR_end;
int XOR_beg, XOR_end;
int NOT_beg, NOT_end;
int LSL_beg, LSL_end;
int LSR_beg, LSR_end;
int Load_beg, Load_end;
int Store_beg, Store_end;

deque<Instruction> ISSUE_INSTR;
vector<Instruction> INSTR_vector;
deque<int> LSBuff;
Instruction I_input;
Instruction I_issue;

int ISSUE(Instruction I, Reservation_Station* RS, vector<RegisterStatus> &RegStatus, vector<int> &Registers);
void EXECUTE(Reservation_Station* RS, vector<RegisterStatus> &RegStatus, vector<int> &Registers);
void WRITE_BACK(Reservation_Station* RS, vector<RegisterStatus> &RegStatus, vector<int> &Registers);
void format_INSTR(string str, Instruction &I);
void Print_RS(Reservation_Station*);
void Print_INSTR();

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    ISSUE_INSTR.clear();
    INSTR_vector.clear();
    LSBuff.clear();
    string str;
    vector<int> Registers(Num_reg_count);
    //INITIAL VALUES OF REGISTERS
    for(int i=0; i<Num_reg_count ; ++i){
        cin>>Registers[i];
    }
    Registers[0]=ZERO_REG;
    vector<RegisterStatus> RegStatus(Num_reg_count);
    for(int i=0; i<Num_reg_count; ++i){
        RegStatus[i]=RegisterStatus(RegStatusEmpty);
    }
    //Reservation Stations
    Reservation_Station* RS=new Reservation_Station[Tot_RS];
    ADD_beg=0;
    for(int i=0; i<Num_ADD_RS; ++i){
        RS[i]=Reservation_Station(AddOp, OperantINIT);
    }
    int sum=Num_ADD_RS;
    ADD_end=sum;
    MUL_beg=sum;
    for(int i=0; i<Num_MUL_RS; ++i){
        RS[sum+i]=Reservation_Station(MulOp, OperantINIT);
    }
    sum+=Num_MUL_RS;
    MUL_end=sum;
    DIV_beg=sum;
    for(int i=0; i<Num_DIV_RS; ++i){
        RS[sum+i]=Reservation_Station(DivOp, OperantINIT);
    }
    sum+=Num_DIV_RS;
    DIV_end=sum;
    AND_beg=sum;
    for(int i=0; i<Num_AND_RS; ++i){
        RS[sum+i]=Reservation_Station(ANDop, OperantINIT);
    }
    sum+=Num_AND_RS;
    AND_end=sum;
    OR_beg=sum;
    for(int i=0; i<Num_OR_RS; i++){
        RS[sum+i]=Reservation_Station(OROp, OperantINIT);
    }
    sum+=Num_OR_RS;
    OR_end=sum;
    XOR_beg=sum;
    for(int i=0; i<Num_XOR_RS; ++i){
        RS[sum+i]=Reservation_Station(XOROp, OperantINIT);
    }
    sum+=Num_XOR_RS;
    XOR_end=sum;
    NOT_beg=sum;
    for(int i=0; i<Num_NOT_RS; ++i){
        RS[sum+i]=Reservation_Station(NOTOp, OperantINIT);
    }
    sum+=Num_NOT_RS;
    NOT_end=sum;
    LSL_beg=sum;
    for(int i=0; i<Num_LSL_RS; ++i){
        RS[sum+i]=Reservation_Station(LSLOp, OperantINIT);
    }
    sum+=Num_LSL_RS;
    LSL_end=sum;
    LSR_beg=sum;
    for(int i=0; i<Num_LSR_RS; ++i){
        RS[sum+i]=Reservation_Station(LSROp, OperantINIT);
    }
    sum+=Num_LSR_RS;
    LSR_end=sum;
    Load_beg=sum;
    for(int i=0; i<Num_Load_RS; ++i){
        RS[sum+i]=Reservation_Station(LoadOp, OperantINIT);
    }
    sum+=Num_Load_RS;
    Load_end=sum;
    Store_beg=sum;
    for(int i=0; i<Num_Store_RS; ++i){
        RS[sum+i]=Reservation_Station(StoreOp, OperantINIT);
    }
    Store_end=sum+Num_Store_RS;
    for(int i=0; i<Mem_size; ++i){
        MemStatus[i]=MemoryStatus(MemoryStatusEmpty);
    }
    cin>>Total_Instr;
    cin.ignore();
    cout<<"*****************************************\n";
    cout<<"INTIAL_STATUS\n";
    cout<<"Registers: ";
    for(int i=0; i<Num_reg_count; ++i){
        cout<<Registers[i]<<"  ";
    }
    cout<<endl;
    cout<<"Memory: \n";
    for(int i=0; i<Mem_size; ++i){
        cout<<i<<": "<<Memory[i]<<"| ";
    }
    cout<<"*****************************************\n";
    do{
        clock_++;
        if(getline(cin, str)){
            format_INSTR(str, I_input);
            ISSUE_INSTR.push_back(I_input);
            INSTR_vector.push_back(I_input);
            I_issue=ISSUE_INSTR.front();
            if(ISSUE(I_issue, RS, RegStatus, Registers)==2){
                ISSUE_INSTR.pop_front();
                currINST_ISSUE++;
            }
        }
        EXECUTE(RS, RegStatus, Registers);
        WRITE_BACK(RS, RegStatus, Registers);
        if(Total_Instr==Tot_Write_Back){
            Done=true;
        }
        //cout<<clock_<<" : "<<Total_Instr<<" "<<Tot_Write_Back<<" "<<str<<" "<<Done<<endl;
        //Print_RS(RS);
        //Print_INSTR();
    }while(!Done);
    cout<<"*****************************************\n";
    cout<<"Clock: "<<clock_<<endl;
    Print_INSTR();
    cout<<"*****************************************\n";
    cout<<"FINAL_STATUS\n";
    cout<<"Registers: ";
    for(int i=0; i<Num_reg_count; ++i){
        cout<<Registers[i]<<"  ";
    }
    cout<<endl;
    cout<<"Memory: \n";
    for(int i=0; i<Mem_size; ++i){
        cout<<i<<": "<<Memory[i]<<"| ";
    }
    cout<<"\n*****************************************\n";
    return 0;
}


int ISSUE(Instruction I, Reservation_Station* RS, vector<RegisterStatus> &RegStatus, vector<int> &Registers){
    int r=I.op;
    bool rsAvail=false;
    switch (r)
    {
        case AddOp:
        case SubOp:
            for(int i=ADD_beg; i<ADD_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case MulOp:
            for(int i=MUL_beg; i<MUL_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case DivOp:
        case ModOp:
            for(int i=DIV_beg; i<DIV_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case ANDop:
            for(int i=AND_beg; i<AND_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case OROp:
            for(int i=OR_beg; i<OR_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case XOROp:
            for(int i=XOR_beg; i<XOR_beg; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case NOTOp:
            for(int i=NOT_beg; i<NOT_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case LSLOp:
            for(int i=LSL_beg; i<LSL_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case LSROp:
            for(int i=LSR_beg; i<LSR_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            break;
        case LoadOp:
            for(int i=Load_beg; i<Load_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            ////cout<<"!!!!!!! "<<INSTR_vector[currINST_ISSUE-1].rs<<" "<<INSTR_vector[currINST_ISSUE-1].rt<<endl;
            if(MemStatus[INSTR_vector[currINST_ISSUE-1].rs + INSTR_vector[currINST_ISSUE-1].rt].Qi==MemoryStatusEmpty){
                RS[r].Qk=OperandAvail;
            }
            else{
                RS[r].Qk=MemStatus[INSTR_vector[currINST_ISSUE-1].rs + INSTR_vector[currINST_ISSUE-1].rt].Qi;
            }
            RS[r].Vj=INSTR_vector[currINST_ISSUE-1].rs;
            RS[r].Vk=INSTR_vector[currINST_ISSUE-1].rt;
            RS[r].Qj=OperandAvail;
            RS[r].isBusy=true;
            RS[r].ISSUE_Lat=0;
            RS[r].instNum=currINST_ISSUE-1;
            INSTR_vector[currINST_ISSUE-1].issueClock=clock_;
            RegStatus[INSTR_vector[currINST_ISSUE-1].rd].Qi=r;
            return 2;
        case StoreOp:
            for(int i=Store_beg; i<Store_end; ++i){
                if(!RS[i].isBusy){
                    r=i;
                    RS[i].op=I.op;
                    rsAvail=true;
                    break;
                }
            }
            if(!rsAvail)
                return 1;
            //Dest=rs+rt
            //Source=rd
            if(RegStatus[INSTR_vector[currINST_ISSUE-1].rd].Qi==RegStatusEmpty){
                RS[r].Vj=Registers[INSTR_vector[currINST_ISSUE-1].rd];
                RS[r].Qj=OperandAvail;
            }
            else{
                RS[r].Qj=RegStatus[INSTR_vector[currINST_ISSUE-1].rd].Qi;
            }
            RS[r].Vk=INSTR_vector[currINST_ISSUE-1].rs+INSTR_vector[currINST_ISSUE-1].rt;
            RS[r].Qk=OperandAvail;
            RS[r].isBusy=true;
            RS[r].ISSUE_Lat=0;
            RS[r].instNum=currINST_ISSUE-1;
            INSTR_vector[currINST_ISSUE-1].issueClock=clock_;
            MemStatus[INSTR_vector[currINST_ISSUE-1].rs+ INSTR_vector[currINST_ISSUE-1].rt].Qi=r;
            return 2;
        default:
            break;
    }
    if(RegStatus[INSTR_vector[currINST_ISSUE-1].rs].Qi == RegStatusEmpty){
        RS[r].Vj = Registers[INSTR_vector[currINST_ISSUE-1].rs];
        RS[r].Qj = OperandAvail;
    }
    else{
        RS[r].Qj = RegStatus[INSTR_vector[currINST_ISSUE-1].rs].Qi;
    }
    if(INSTR_vector[currINST_ISSUE-1].imm!=0){
        RS[r].Vk=INSTR_vector[currINST_ISSUE-1].imm;
        RS[r].Qk=OperandAvail;
    }
    else if(RegStatus[INSTR_vector[currINST_ISSUE-1].rt].Qi == RegStatusEmpty){
        RS[r].Vk = Registers[INSTR_vector[currINST_ISSUE-1].rt];
        RS[r].Qk = OperandAvail;
    }
    else{
        RS[r].Qk = RegStatus[INSTR_vector[currINST_ISSUE-1].rt].Qi;
    }
    RS[r].isBusy=true;
    RS[r].ISSUE_Lat=0;
    RS[r].instNum=currINST_ISSUE-1;
    INSTR_vector[currINST_ISSUE-1].issueClock=clock_;
    RegStatus[INSTR_vector[currINST_ISSUE-1].rd].Qi=r;

    return 2;
}

void EXECUTE(Reservation_Station* RS, vector<RegisterStatus> &RegStatus, vector<int> &Registers){
    bool startExec=false;
    for(int i=0; i<Tot_RS; ++i){
        startExec=false;
        if(RS[i].isBusy){
            if(RS[i].ISSUE_Lat>=ISSUE_Lat){
                if(RS[i].Qj==OperandAvail && RS[i].Qk==OperandAvail){
                    if(INSTR_vector[RS[i].instNum].executeClockBegin==0){
                        INSTR_vector[RS[i].instNum].executeClockBegin=clock_;
                    }
                    switch (RS[i].op)
                    {
                    case AddOp:
                        if(RS[i].lat==AddLat){
                            RS[i].result=RS[i].Vj+RS[i].Vk;
                            startExec=true;
                        }
                        break;
                    case SubOp:
                        if(RS[i].lat==AddLat){
                            RS[i].result=RS[i].Vj-RS[i].Vk;
                            startExec=true;
                        }
                        break;
                    case MulOp:
                        if(RS[i].lat==MulLat){
                            RS[i].result=RS[i].Vj*RS[i].Vk;
                            startExec=true;
                        }
                        break;
                    case DivOp:
                        if(RS[i].lat==DivLat){
                            RS[i].result=RS[i].Vj/RS[i].Vk;
                            startExec=true;
                        }
                        break;
                    case ANDop:
                        if(RS[i].lat==ANDLat){
                            RS[i].result=(RS[i].Vj&RS[i].Vk);
                            startExec=true;
                        }
                        break;
                    case OROp:
                        if(RS[i].lat==ORLat){
                            RS[i].result=(RS[i].Vj|RS[i].Vk);
                            startExec=true;
                        }
                        break;
                    case XOROp:
                        if(RS[i].lat==XORLat){
                            RS[i].result=(RS[i].Vj^RS[i].Vk);
                            startExec=true;
                        }
                        break;
                    case NOTOp:
                        if(RS[i].lat==NOTLat){
                            RS[i].result=!(RS[i].Vj);
                            startExec=true;
                        }
                        break;
                    case LSLOp:
                        if(RS[i].lat==LSLLat){
                            RS[i].result=RS[i].Vj<<RS[i].Vk;
                            startExec=true;
                        }
                        break;
                    case LSROp:
                        if(RS[i].lat==LSRLat){
                            RS[i].result=RS[i].Vj>>RS[i].Vk;
                            startExec=true;
                        }
                        break;
                    case ModOp:
                        if(RS[i].lat==DivLat){
                            RS[i].result=RS[i].Vj%RS[i].Vk;
                            startExec=true;
                        }
                        break;
                    case LoadOp:
                        if(RS[i].lat==LoadLat){
                            RS[i].result=Memory[RS[i].Vj+RS[i].Vk];
                            startExec=true;
                        }
                        break;
                    case StoreOp:
                        if(RS[i].lat==StoreLat){
                            RS[i].result=Registers[RS[i].Vj];
                            startExec=true;
                        }
                        break;
                    default:
                        break;
                    }
                    if(startExec){
                        RS[i].resultReady=true;
                        RS[i].lat=0;
                        RS[i].ISSUE_Lat=0;
                        INSTR_vector[RS[i].instNum].executeClockEnd=clock_;
                    }
                    else{
                        RS[i].lat++;
                    }
                }
            }
            else{
                RS[i].ISSUE_Lat++;
            }
        }
    }
}

void WRITE_BACK(Reservation_Station* RS, vector<RegisterStatus> &RegStatus, vector<int> &Registers){
    for(int i=0; i<Tot_RS; ++i){
        if(RS[i].resultReady){
            if(RS[i].WRITEBACK_Lat==WRITE_BACK_Lat){
                if(INSTR_vector[RS[i].instNum].writebackClock==0){
                    INSTR_vector[RS[i].instNum].writebackClock=clock_;
                }
                if(RS[i].op==StoreOp){
                    for(int j=0; j<Mem_size; ++j){
                        if(MemStatus[j].Qi==i){
                            Memory[j]=RS[i].result;
                            MemStatus[j]=MemoryStatusEmpty;
                        }
                    }
                    for(int j=Load_beg; j<Load_end; ++j){
                        if(RS[j].Qk==i){
                            RS[j].Qk=OperandAvail;
                        }
                    }
                }
                else{
                    for(int j=0; j<Num_reg_count; ++j){
                        if(RegStatus[j].Qi==i){
                            Registers[j]=RS[i].result;
                            RegStatus[j].Qi=RegStatusEmpty;
                        }
                    }
                    for(int j=0; j<Tot_RS; ++j){
                        if(RS[j].Qj==i){
                            RS[j].Vj=RS[i].result;
                            RS[j].Qj=OperandAvail;
                        }
                        if(RS[j].Qk==i){
                            RS[j].Qk=OperandAvail;
                            RS[j].Vk=RS[i].result;
                        }
                    }
                }
                RS[i].resultReady=false;
                RS[i].isBusy=false;
                RS[i].Qj=OperantINIT;
                RS[i].Qk=OperantINIT;
                RS[i].Vj=0;
                RS[i].Vk=0;
                RS[i].WRITEBACK_Lat=0;
                Tot_Write_Back++;
            }
            else{
                RS[i].WRITEBACK_Lat++;
            }
        }
    }
}
void format_INSTR(string s, Instruction &I){
    string opcode="";
    int i=0;
    for(; i<s.length(); ++i){
        if(s[i]!=' '){
            opcode+=s[i];
        }
        else{
            break;
        }
    }
    int OP=0;
    if(opcode=="ADD"){
        OP=0;
    }
    else if(opcode=="SUB"){
        OP=1;
    }
    else if(opcode=="MUL"){
        OP=2;
    }
    else if(opcode=="DIV"){
        OP=3;
    }
    else if(opcode=="AND"){
        OP=4;
    }
    else if(opcode=="OR"){
        OP=5;
    }
    else if(opcode=="XOR"){
        OP=6;
    }
    else if(opcode=="NOT"){
        OP=7;
    }
    else if(opcode=="LSL"){
        OP=8;
    }
    else if(opcode=="LSR"){
        OP=9;
    }
    else if(opcode=="MOD"){
        OP=ModOp;
    }
    else if(opcode=="LOAD" || opcode=="STORE"){
        OP=(opcode=="LOAD")?(LoadOp):(StoreOp);
        //rd, int, int
        int rd=0, rs=0, rt=0;
        for(; i<s.length(); ++i){
            i++;
            if(s[i]=='r'){
                i++;
                while(s[i]>='0' && s[i]<='9'){
                    rd=(rd*10+(s[i]-'0'));
                    i++;
                }
                break;
            }
        }
        for(; i<s.length(); ++i){
            while(rs==0 && (s[i]<'0' || s[i]>'9')){
                i++;
                while(s[i]>='0' && s[i]<='9'){
                    rs=(rs*10+(s[i]-'0'));
                    i++;
                }
            }
            if(rs!=0)   break;
        }
        for(; i<s.length(); ++i){
            if(s[i]<='9' && s[i]>='0'){
                rt=(rt*10+(s[i]-'0'));
            }
        }
        I=Instruction(rd, rs, rt, OP);
        return;
    }
    else{
        cout<<"Unidentied Instruction OPCODE for I"<<currINST_ISSUE<<" "<<s<<" "<<opcode<<endl;
    }
    int rd_in=0, rs_in=0, rt_in=0;
    for(; i<s.length(); ++i){
        if(s[i]=='r'){
            i++;
            while(s[i]>='0' && s[i]<='9'){
                rd_in=(rd_in*10+(s[i]-'0'));
                i++;
            }
            break;
        }
    }
    bool isImm=false;
    int imm=0;
    for(; i<s.length(); ++i){
        if(s[i]=='r'){
            i++;
            while(s[i]>='0' && s[i]<='9'){
                rs_in=(rs_in*10+(s[i]-'0'));
                i++;
            }
            break;
        }
    }
    for(; i<s.length(); ++i){
        if(s[i]=='r'){
            i++;
            while(s[i]>='0' && s[i]<='9'){
                rt_in=(rt_in*10+(s[i]-'0'));
                i++;
            }
            break;
        }
        if(s[i]=='i'){
            isImm=true;
            i++;
            while(s[i]>='0' && s[i]<='9')
            {
                imm=(imm*10+(s[i]-'0'));
                //cout<<isImm<<" "<<imm<<endl;
                i++;
            }
            break;
        }
    }
    I=Instruction(rd_in, rs_in, rt_in, OP, imm);
}

void Print_RS(Reservation_Station* RS){
    cout<<"----------------------\n";
    cout<<"Cycle: "<<clock_<<endl;
    for(int i=0; i<Tot_RS; ++i){
        cout<<"#"<<i<<" isBusy: "<<RS[i].isBusy<<" OP:"<<RS[i].op<<" Vj:"<<RS[i].Vj<<" Vk:"<<RS[i].Vk<<" Qj"<<RS[i].Qj<<" Qk"<<RS[i].Qk<<" instNum:"<<RS[i].instNum<<" lat:"<<RS[i].lat<<endl;
    }
}

void Print_INSTR(){
    cout<<"=============================================================================\n";
    for(int i=0; i<Total_Instr; ++i){
        cout<<"#"<<i<<"| OP:"<<INSTR_vector[i].op<<"| Issuebeg:"<<INSTR_vector[i].issueClock<<"| EXECbeg:"<<INSTR_vector[i].executeClockBegin<<"| ExecEnd:"<<INSTR_vector[i].executeClockEnd<<"| Writeback:"<<INSTR_vector[i].writebackClock<<"|"<<endl;
    }
    cout<<"=============================================================================\n";
}
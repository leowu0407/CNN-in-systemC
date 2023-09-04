#include "systemc.h"
#include "define.h"
#include <iostream>
#include <iomanip>
using namespace std;

// vvvvv put your code here vvvvv

SC_MODULE( Convolution_Pooling ) {	
	sc_in_clk clk;
	sc_in < bool > rst;
	
	sc_out < bool > rom_rd;
	sc_out < sc_uint<16> > rom_addr;
	sc_in < DATA_TYPE > rom_data_out;
  sc_out < bool > rom_1;
	
	sc_out < bool > ram_wr;
	sc_out < sc_uint<16> > ram_addr;
	sc_out < DATA_TYPE > ram_data_in;
  sc_out < bool > ram_1;
	
  sc_out < sc_uint<16> > end;  
   
	  void run();
	
    int step=0;
 
    int i=0,j=0;
    
    
    MUL_DATA_TYPE conv_1[6][24*24];
    
    int start = 0;
    
    
    DATA_TYPE conv_1_kernel[6][26]; 
    DATA_TYPE input[784];
    
    int addr=0;
    
    MUL_DATA_TYPE conv_1_result=0;
    
    int c=0,layer=0;
    int num=0;
    int channel = 0,channel_1 = 0;
    int count=0;
    int conv_1_index=0;
    
    bool get_kernel=true,get_input = false;
    
    MUL_DATA_TYPE maxpooling_1[6][144];
    MUL_DATA_TYPE max;
    
    int col[25]={0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4};
    int row[25]={0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4};
    
    MUL_DATA_TYPE conv_2[16][64];
    DATA_TYPE conv_2_kernel[16][6][25];
    DATA_TYPE conv_2_bias[16];
    
    int channel_2=0;
    int ix=0,iy=0,iz=0,ibias=0;
    
    MUL_DATA_TYPE conv_2_result=0;
    
 
	SC_CTOR( Convolution_Pooling )
	{
		SC_METHOD( run );
		sensitive << clk.pos();
	}
};
SC_MODULE( Dense ) {	
	sc_in_clk clk;
	sc_in < bool > rst;
	
	sc_out < bool > rom_rd;
	sc_out < sc_uint<16> > rom_addr;
	sc_in < DATA_TYPE > rom_data_out;
  sc_out < bool > rom_2;
	
	sc_out < bool > ram_wr;
	sc_out < sc_uint<16> > ram_addr;
	sc_in < DATA_TYPE > ram_data_out;
  sc_out < bool > ram_2;
	
	sc_out < DATA_TYPE > result;
	sc_out < bool > output_valid;
	
  sc_in < sc_uint<16> > end; 
  
	  void run();
     
    int e=0,addr=0;
	  int channel_1 = 0;
    int step=5;
    int count=0;
    int c=0,ix=0;
    int i,j,x=0,y=0;
    int layer=0;
    
    bool read_data = true;
    bool get_kernel=true;
    bool flatten = true;
   
    MUL_DATA_TYPE maxpooling_2[16][16];
    MUL_DATA_TYPE input[256];
    
    
    DATA_TYPE dense_1_weight[120][257];
    MUL_DATA_TYPE dense_1[120];
    MUL_DATA_TYPE dense_1_result[120];
    
    
    DATA_TYPE dense_2_weight[84][121];
    MUL_DATA_TYPE dense_2[84];
    MUL_DATA_TYPE dense_2_result[84];
    
    
    DATA_TYPE dense_3_weight[10][85];
    DATA_TYPE dense_3[10];
    MUL_DATA_TYPE dense_3_result[10];
    
    
 
	SC_CTOR( Dense )
	{
        for(i=0; i<120; i++){
            dense_1_result[i]=0;
        }
        for(i=0; i<84; i++){
            dense_2_result[i]=0;
        }
        for(i=0; i<10; i++){
            dense_3_result[i]=0;
        }
 
 
		SC_METHOD( run );
		sensitive << clk.pos();
	}
};

SC_MODULE( Controller ) {	
	sc_in_clk clk;
	sc_in < bool > rst;
	
	sc_in < bool > rom_rd_1;
  sc_in < bool > rom_rd_2;
  
	sc_in < sc_uint<16> > rom_addr_1;
  sc_in < sc_uint<16> > rom_addr_2;
  
  sc_in < bool > rom_1;
  sc_in < bool > rom_2;
  
  sc_out < bool > rom_rd;  
  sc_out < sc_uint<16> > rom_addr;
  
  sc_in < bool > ram_wr_1;
  sc_in < bool > ram_wr_2;
  
	sc_in < sc_uint<16> > ram_addr_1;
  sc_in < sc_uint<16> > ram_addr_2;
  
  sc_in < bool > ram_1;
  sc_in < bool > ram_2;
  sc_out < bool > ram_wr;  
  sc_out < sc_uint<16> > ram_addr;
  
  void run();
 
	SC_CTOR( Controller )
	{
    SC_METHOD( run );
		sensitive << clk.pos();
	}
};



SC_MODULE( LeNet ) {	
	sc_in_clk clk;
	sc_in < bool > rst;
	
	sc_out < bool > rom_rd;
	sc_out < sc_uint<16> > rom_addr;
	sc_in < DATA_TYPE > rom_data_out;
	
	sc_out < bool > ram_wr;
	sc_out < sc_uint<16> > ram_addr;
	sc_in < DATA_TYPE > ram_data_out;
	sc_out < DATA_TYPE > ram_data_in;
	
	sc_out < DATA_TYPE > result;
	sc_out < bool > output_valid;
  
	sc_signal < bool > rom_rd_1;
  sc_signal < sc_uint<16> > rom_addr_1;
  sc_signal < sc_uint<16> > rom_addr_2;
  sc_signal < sc_uint<16> > ram_addr_1;
  sc_signal < sc_uint<16> > ram_addr_2;
  sc_signal < bool > rom_1;
  sc_signal < bool > ram_1; 
  sc_signal < bool > rom_2;
  sc_signal < bool > ram_2; 
  sc_signal < bool > rom_rd_2;
  sc_signal < bool > ram_wr_1;
  sc_signal < bool > ram_wr_2;
  sc_signal < sc_uint<16> > end;
  Convolution_Pooling *C_P_1;
  Dense *Dense_1;
  Controller *Controller_1;  
	 
	SC_CTOR( LeNet )
	{
    C_P_1 = new Convolution_Pooling("C_P_1");
    Dense_1 = new Dense("Dense_1"); 
		(*C_P_1)(clk,rst,rom_rd_1,rom_addr_1,rom_data_out,rom_1,ram_wr_1,ram_addr_1,ram_data_in,ram_1,end);
    (*Dense_1)(clk,rst,rom_rd_2,rom_addr_2,rom_data_out,rom_2,ram_wr_2,ram_addr_2,ram_data_out,ram_2,result,output_valid,end); 
    (*Controller_1)(clk,rst,rom_rd_1,rom_rd_2,rom_addr_1,rom_addr_2,rom_1,rom_2,rom_rd,rom_addr,ram_wr_1,ram_wr_2,ram_addr_1,ram_addr_2,ram_1,ram_2,ram_wr,ram_addr);  
	}
};

// ^^^^^ put your code here ^^^^^
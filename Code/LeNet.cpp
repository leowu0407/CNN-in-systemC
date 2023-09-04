#include "LeNet.h"
// vvvvv put your code here vvvvv

void Convolution_Pooling::run(){
	
    if(step == 0){
      rom_1.write(true);
      ram_1.write(true);
      step = step + 1;
      
    }
    else if(step==1){  // convolution 1
      
            if(get_kernel){
                
                if(c < 159){
                  rom_rd.write(1);
                  rom_addr.write(c);
                  if(c >=3){
                  conv_1_kernel[ix][c-3-26*ix] = rom_data_out.read();
                  }
                  if(c % 26 == 3 && c != 3)
                    ix++;
                  c++;
                }
                
                else{
                  get_kernel = false;
                  get_input = true;
                  ix = 0;
                  c=0;
                }
                
            }
            
            else if(get_input){
                if(c < 787){
                  rom_rd.write(1);
                  rom_addr.write(44426+c);
                  if(c >=3){
                  input[c-3] = rom_data_out.read();
                  }
                  c++;
                }
                else{
                  get_input = false;
                  c=0;
                }
          }
          else{
            if(channel_1 < 6){
              conv_1[channel_1][count] = conv_1_kernel[channel][0] * input[ix] + conv_1_kernel[channel][1] * input[ix+1] + conv_1_kernel[channel][2] * input[ix+2] + conv_1_kernel[channel][3] * input[ix+3] + conv_1_kernel[channel][4] * input[ix+4] + conv_1_kernel[channel][5] * input[ix+28] + conv_1_kernel[channel][6] * input[ix+29] + conv_1_kernel[channel][7] * input[ix+30] + conv_1_kernel[channel][8] * input[ix+31] + conv_1_kernel[channel][9] * input[ix+32] + conv_1_kernel[channel][10] * input[ix+56] + conv_1_kernel[channel][11] * input[ix+57] + conv_1_kernel[channel][12] * input[ix+58] + conv_1_kernel[channel][13] * input[ix+59] + conv_1_kernel[channel][14] * input[ix+60] + conv_1_kernel[channel][15] * input[ix+84] + conv_1_kernel[channel][16] * input[ix+85] + conv_1_kernel[channel][17] * input[ix+86] + conv_1_kernel[channel][18] * input[ix+87] + conv_1_kernel[channel][19] * input[ix+88] + conv_1_kernel[channel][20] * input[ix+112] + conv_1_kernel[channel][21] * input[ix+113] + conv_1_kernel[channel][22] * input[ix+114] + conv_1_kernel[channel][23] * input[ix+115] + conv_1_kernel[channel][24] * input[ix+116] + conv_1_kernel[channel][25];
              #ifdef fixed_DATA_TYPE
              conv_1[channel_1][count] = conv_1[channel_1][count] / 1024;
              #endif
              if(conv_1[channel_1][count] < 0)
                conv_1[channel_1][count] = 0;
                num = num + 1;
              ix = ix + 1;
              if(ix % 28 == 24)
                ix = ix + 4;
              count = count + 1;
              if(count == 576){
                count = 0;
                channel = channel + 1;
                channel_1 = channel_1  + 1;
                ix = 0;
              }
              if(channel_1 == 6){
              c = 0;
              count = 0;
              ix = 0;
              step = 2;
              channel = 0;
              channel_1 = 0;
              get_kernel = true;
              }
            
            }
            
            
          }
          
          
          
    }  // end step1
    
    else if(step == 2){  // pooling 1
          
          for(i = 0; i < 6; i++)
          {
            count = 0;
            for(j = 0; j < 551; j+=2)
            {
              max = conv_1[i][j];
              if(max < conv_1[i][j+1])
                max = conv_1[i][j+1];
              if(max < conv_1[i][j+24])
                max = conv_1[i][j+24];
              if(max < conv_1[i][j+25])
                max = conv_1[i][j+25];
              if(j % 24 == 22)
                j = j + 24;
              maxpooling_1[i][count] = max;
              count = count + 1;
            }
            if(i == 5)
            {
               step = 3;
               
            }
          }
    }  // end step2
    
    else if(step == 3){  // convolution 2
        if(get_kernel){
                
                if(c < 2419){
                  rom_rd.write(1);
                  rom_addr.write(c+156);
                  if(c >= 3){
                    if(c % 151 == 2 && c != 2){
                        conv_2_bias[ibias] = rom_data_out.read();
                        ibias = ibias + 1;
                    }
                    else{
                      conv_2_kernel[ix][iy][iz] = rom_data_out.read();
                      iz = iz + 1;
                    }
                  }
                  if(iz == 25){
                    iy = iy + 1;
                    iz = 0;
                  }
                  if(iy == 6){
                    iy = 0;
                    ix = ix + 1;
                  }
                  c++;
                }
                
                else{
                  get_kernel = false;
                  ix = 0;
                  iy = 0;
                  iz = 0;
                  ibias = 0;
                  c=0;
                }
                
        }
        else{
          for(layer=0; layer<16; layer++){              
                start = 0;
                count = 0;
                while(start<92){
                    for(i=0; i<6; i++){
                        for(j=0; j<25; j++){
                            conv_2_result += maxpooling_1[i][start + col[j] + row[j]*12] * conv_2_kernel[layer][i][j];
                        }
                    }
                    conv_2_result = conv_2_result + conv_2_bias[layer];
                    #ifdef fixed_DATA_TYPE
                    conv_2_result/=1024;
                    #endif
                    if(conv_2_result<0){
                        conv_2_result=0;
                    }
                    conv_2[layer][count] = conv_2_result;
                    conv_2_result=0;
                    count++;
                    start++;
                    if(start%12==8){
                      start+=4;
                    }
                    
                }
            }
            i=0;
            j=0;
            step = 4;
        }
    }  // end of step3
    
   
   
   // step4 maxpooling 2//
  else if(step == 4)
    { 
              if(i < 16){
                max = conv_2[i][j];
                if(max < conv_2[i][j+1])
                  max = conv_2[i][j+1];
                if(max < conv_2[i][j+8])
                  max = conv_2[i][j+8];
                if(max < conv_2[i][j+9])
                  max = conv_2[i][j+9];
                if(j % 8 == 6)
                  j = j + 8;
                j = j + 2;
                if(j >= 55){
                  i = i + 1;
                  j = 0;
                }
              ram_1.write(true);
              ram_wr.write(false);
              ram_data_in.write(max);
              ram_addr.write(count); 
              count = count + 1;
              num = num + 1;
              
            }
            if(i == 16)
              {
                step = 5;
                get_kernel = true;
                channel_1 = 0;
                ram_wr.write(1);
                end.write(count-2);
                ram_addr.write(0);
                count = 0;
                rom_1.write(false);
                ram_1.write(false);
              }
	}    // end of step4 //

}
void Dense::run(){
  ram_2.write(true);
  rom_2.write(true);
  e = end.read();
  if(read_data){
    if(count <= e+1){
      if(count < 64){
        ram_wr.write(true);
        count = count + 1;
        ram_2.write(true);
        ram_addr.write(count);
      }
      else{
        maxpooling_2[x][y] = ram_data_out.read();
        ram_wr.write(true);
        count = count + 1;
        ram_2.write(true);
        ram_addr.write(count);
      y = y + 1;
      }
      if(y == 16){
        y = 0;
        x = x + 1;
        if(x == 16){
          read_data = false;
          flatten = true;
          x = 0;
          y = 0;
          count = 0;
        }
      }
    }
    
    
    //read_data = false;
  }
  else if(flatten){
      if(count < 256){
        input[count] = maxpooling_2[x][y];
        x++;
        count++;
        if(x == 16){
          y++;
          x = 0;
        }
      }
      else{
      flatten = false;
      step = 5;
      count = 0;
      x = 0;
      y = 0;
    }
    }
  
  else if(step == 5)   // dense 1
	{
		if(channel_1 < 120)
		{
			if(get_kernel)
			{       
        if(c < 30843){
                  rom_rd.write(1);
                  rom_addr.write(c+2572);
                  if(c >=3){
                  dense_1_weight[ix][c-3-257*ix] = rom_data_out.read();
                  }
                  c++;
                  if(c % 257 == 3 && c != 3)
                    ix++;
                    
        }
            
        else{
              
                  get_kernel = false;
                  ix = 0;
                  c=0;
        }
         
	            
	        }
	        else
	        {
	        	for(i = 0; i < 16; i++)
	        	{
	        		for(j = 0; j < 16; j++)
	        		{
	        			dense_1_result[channel_1] = dense_1_result[channel_1] + input[16*i+j] * dense_1_weight[channel_1][j + i * 16];
					    }
				    }
			        	dense_1_result[channel_1] = dense_1_result[channel_1] + dense_1_weight[channel_1][256];
                #ifdef fixed_DATA_TYPE
                dense_1_result[channel_1] = dense_1_result[channel_1] / 1024;
                #endif
                if(dense_1_result[channel_1] < 0){
                    dense_1_result[channel_1] = 0;
                }
                dense_1[channel_1] = dense_1_result[channel_1];
                
				addr = 0;
				channel_1 = channel_1 + 1;
		  	}
		}
		else
		{  
            
			channel_1 = 0;
      ix = 0;
      c = 0;
			step = 6;
			get_kernel = true;
      addr = 0;
      count = 0;
                
		}	
	}  // end of step5 //
    
    else if(step == 6)  // dense 2
    {
    	if(channel_1 < 84)
    	{
			    if(get_kernel)
			    {
	              
             if(c < 10167){
                  rom_rd.write(1);
                  rom_addr.write(c+33412);
                  if(c >=3){
                  dense_2_weight[ix][c-3-121*ix] = rom_data_out.read();
                  //cout << c-3 << " " << conv_1_kernel[ix][c-3-26*ix] << endl;
                  }
                  c++;
                  if(c % 121 == 3 && c != 3)
                    ix++;
                    
            }
            
        else{
                  get_kernel = false;
                  ix = 0;
                  c=0;
        }
          }
	        else
	        {
	        	for(i = 0; i < 120; i++)
	        	{
	        		dense_2_result[channel_1] = dense_2_result[channel_1] + dense_1[i] * dense_2_weight[channel_1][i];
				    }
				    dense_2_result[channel_1] = dense_2_result[channel_1] + dense_2_weight[channel_1][120];
            #ifdef fixed_DATA_TYPE
            dense_2_result[channel_1] = dense_2_result[channel_1] / 1024;
            #endif
                if(dense_2_result[channel_1] < 0){
                    dense_2_result[channel_1] = 0;
                }
            dense_2[channel_1] = dense_2_result[channel_1];
				    addr = 0;
				    channel_1 = channel_1 + 1;
			}
		}
		else
		{
			channel_1 = 0;
      ix = 0;
      c = 0;
			step = 7;
			get_kernel = true;
      addr = 0;
      count = 0;
		}		
	}    // end of step6 //
    
    
    else if(step == 7)  // dense 3
    {
       
            if(get_kernel)
			      {
	              
               if(c < 853){
                  rom_rd.write(1);
                  rom_addr.write(c+43576);
                  if(c >= 3){
                  dense_3_weight[ix][c-3-85*ix] = rom_data_out.read();
                  }
                  c++;
                  if(c % 85 == 3 && c != 3)
                    ix++;
                    
              }
            
              else{
              
                  get_kernel = false;
                  ix = 0;
                  c=0;
                }
            }
            else if(channel_1 < 10){
                if(ix < 84){
                    dense_3_result[channel_1] += (dense_2[ix] * dense_3_weight[channel_1][ix]);

                    ix = ix + 1;
                }
                if(ix == 84){
                  dense_3_result[channel_1] += dense_3_weight[channel_1][84];
                  #ifdef fixed_DATA_TYPE
                  dense_3_result[channel_1]  = dense_3_result[channel_1] / 1024;
                  #endif
                  dense_3[channel_1] = dense_3_result[channel_1];
                  channel_1 = channel_1 + 1;
                  ix = 0;
                }  
                
            }
        
        else
        {
            step = 8;
            count = 0;
            i = 0;
            
        }
    }   // end of step7 //

    else if(step == 8)  // step8
    {
        if(count < 10)
        {
            output_valid.write(true);
            result.write(dense_3[i]);
            count = count + 1;
            i = i + 1;
        }
        else
        {
            step = 0;
        }
    }
}

void Controller::run(){
  if(rom_1.read()){
    rom_rd.write(rom_rd_1.read());
    rom_addr.write(rom_addr_1.read());
    
  }
  else if(rom_2.read()){
    rom_rd.write(rom_rd_2.read());
    rom_addr.write(rom_addr_2.read());
  }
  if(ram_1.read()){
    ram_wr.write(ram_wr_1.read());
    ram_addr.write(ram_addr_1.read());
    
  }
  else if(ram_2.read()){
    ram_wr.write(ram_wr_2.read());
    ram_addr.write(ram_addr_2.read());
  }
}
// ^^^^^ put your code here ^^^^^
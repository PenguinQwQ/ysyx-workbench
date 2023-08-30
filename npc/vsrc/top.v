module top(
  input clk, //输入的时钟
  input rst, //复位信号
    output reg [15:0] led //输出的led信号
);
    reg [31:0] count; //计数器
    always @(posedge clk) begin //上升沿激发，期内时序逻辑都用非阻塞赋值
        if (rst) begin led <= 1; count <= 0; end //若复位，则重置count分频计数器，led灯信号重置为1
    	else begin //否则
            if (count == 0) led <= {led[14:0], led[15]}; //在分频器经过50w次累加重置为0时，将led信号左移（在NVBoard上，LSD到MSD是左移的过程） 
            count <= (count >= 5000000 ? 32'b0 : count + 1); //分频器累加
    end
  end
endmodule
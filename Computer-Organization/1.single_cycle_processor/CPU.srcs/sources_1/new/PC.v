`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:40:41 03/19/2016 
// Design Name: 
// Module Name:    PC 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module PC( clk, reset, PCWre, nextPC, curPC);
    input clk, reset, PCWre;    // clkΪʱ���źţ�resetΪ�����źţ�PCWreΪPCʹ���ź�
    input [31:0] nextPC;        // nextPCΪ�¸�PCֵ
    output reg [31:0] curPC;   // ��ǰPCֵ

	// �����ش���
    always@(posedge clk) begin
        if( reset == 0)
            curPC <= 0;  // PC��ֵΪ0, ��0��ַ��ʼ
        else if (PCWre)
			curPC = nextPC;  // ����ָ��ĵ�ַ
    end
endmodule
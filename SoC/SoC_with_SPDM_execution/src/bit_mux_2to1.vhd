entity bit_mux_2to1 is
port(
	A,B: in bit;
	selector: in bit;
	mux_output: out bit
	);
end entity;

architecture multiplexer_bit of bit_mux_2to1 is
begin
	with selector select
		mux_output <= A when '0',
		              B when '1';
end architecture;	


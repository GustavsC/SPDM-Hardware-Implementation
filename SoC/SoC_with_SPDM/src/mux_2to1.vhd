entity mux_2to1 is
port(
	A,B: in bit_vector(31 downto 0);
	selector: in bit;
	mux_output: out bit_vector(31 downto 0)
	);
end entity;

architecture multiplexer of mux_2to1 is
begin
	with selector select
		mux_output <= A when '0',
		              B when '1';
end architecture;	


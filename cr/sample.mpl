program sample11pp;
var sum : integer;
var data : integer;
var n : integer;
procedure goukei(n, s : integer);
  var data : integer;
begin
  s:=0;
  while n > 0 do begin
    readln(data);
    s := s + data;
    n := n - 1
  end
end;

begin
  call goukei(n * 2, sum);
end.
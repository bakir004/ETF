using LinearAlgebra

function tabela(A, b, c)
  m = size(A, 1);
  tabela = [b A I(m); [0 transpose(c) transpose(zeros(m))]]
  return tabela;
end

print(tabela([1 1; 2 1], [4, 6], [3, 2]))
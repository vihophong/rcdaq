{
TF1 * g1;
g1 = new TF1("g1","gaus",-40,40);
plock();
H1->Fit("g1");
prelease();
delete g1;
}

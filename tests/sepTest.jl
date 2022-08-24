
include("sep.lj")

import .sep

sep.Init("start.xyz")

for n=1:1000

    sep.Reset()
    
    sep.ForceLJ("AA", [2.5, 1.0, 1.0, 1.0])
    sep.Integrate()

    epot, ekin = sep.GetEnergies()

    println(epot/1000, " ", ekin/1000)
    
end

sep.Save("output.xyz");

sep.Close()





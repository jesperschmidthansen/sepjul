
include("../module/sep.jl")

import .sep

function testLJ()
    sep.Init("start.xyz")
    
    for n=1:1000
        
        sep.Reset()
        
        sep.ForceLJ("AA", [2.5, 1.0, 1.0, 1.0])
        sep.Integrate()

        epot, ekin = sep.GetEnergies()
        
        println(epot/1000, " ", ekin/1000)
        
    end

    sep.Save("output.xyz", "C");
    
    sep.Close()
end

function testButane()
    
    sep.Init("sysButane.xyz", "sysButane.top")
    sep.SetExclusionRule("molecule")
    
    for n=1:1000
        
        sep.Reset()
        
        sep.ForceLJ("CC", [2.5, 1.0, 1.0, 1.0])
        sep.ForceBondHarmonic(0, 0.4, 33615.0)
        sep.ForceAngleCossq(0, 1.9, 866.0)
        sep.ForceTorsion(0,[15.5000,  20.3050, -21.9170, -5.1150,  43.8340, -52.6070])
    
        sep.Leapfrog()
        sep.RelaxTemp('C', 4.0, 0.01);
        
        epot, ekin = sep.GetEnergies()
        
        println(epot/4000, " ", ekin/4000)
        
    end

    sep.Save("output.xyz", "C");
    
    sep.Close()
end






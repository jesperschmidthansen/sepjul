module sep

using Libdl

strpath="/home/jschmidt/software/sepjul/module/libseptojul.so"
lib = dlopen(strpath)

## Init and close/clear block
function Init(xyzfile)

    ccall(dlsym(lib, "sepInit_1"), Cvoid, (Cstring,), xyzfile)

end

function Init(xyzfile, topfile)

    ccall(dlsym(lib, "sepInit_2"), Cvoid, (Cstring, Cstring), xyzfile, topfile)
    
end

function Close()

    ccall(dlsym(lib, "sepClear"), Cvoid, ())

end

# Reset forces and return structure
function Reset()

    ccall(dlsym(lib, "sepReset"), Cvoid, ())

end

# Force functions
function ForceLJ(types, params)

    ccall(dlsym(lib, "sepForceLJ"), Cvoid, (Cstring, Ptr{Cdouble}), types, params)

end


function ForceBondHarmonic(btype, blength, bconstant)

    ccall(dlsym(lib, "sepBondHarmonic"), Cvoid, (Cint, Cdouble, Cdouble),
          btype, blength, bconstant)
 
end

function ForceAngleCossq(atype, angle, aconstant)

    ccall(dlsym(lib, "sepAngleCossq"), Cvoid, (Cint, Cdouble, Cdouble),
          atype, angle, aconstant)
 
end


function ForceTorsion(ttype, params)

    ccall(dlsym(lib, "sepForceTorsion"), Cvoid, (Cint, Ptr{Cdouble}), ttype, params)
 
end

function ForceCoulombSF(cutoff)

    ccall(dlsym(lib, "sepForceCoulombSF"), Cvoid, (Cdouble,), cutoff)
    
end

# Integration
function LeapFrog()

    ccall(dlsym(lib, "sepLeapFrog"), Cvoid, ())

end

# Thermostating methods
function RelaxTemp(type, desiredTemp, tau)

    ccall(dlsym(lib, "sepRelaxTemp"), Cvoid, (Cchar, Cdouble, Cdouble), type, desiredTemp, tau)
 
end

# Save
function Save(filename)

    ccall(dlsym(lib, "sepSave"), Cvoid, (Cstring,), filename)

end

# Get functions
function GetEnergies()

    retval = zeros(2)
	 
    ccall(dlsym(lib, "sepGetEnergies"), Cvoid, (Ptr{Cdouble},), retval)

    epot = retval[1]
    ekin = retval[2]
    
    return epot, ekin
end

function GetNumbParticles()

    npart = ccall(dlsym(lib, "sepGetNumbParticles"), Cint, ())

    return npart 	  
end

function GetPositions()

    npart = ccall(dlsym(lib, "sepGetNumbParticles"), Cint, ())
    
    x = zeros(npart)
    y = zeros(npart)
    z = zeros(npart)

    ccall(dlsym(lib, "sepGetPositions"), Cvoid,
                (Ptr{Cdouble}, Ptr{Cdouble}, Ptr{Cdouble}), x, y, z)
    
    return x,y,z
end


function GetVelocities()

    npart = ccall(dlsym(lib, "sepGetNumbParticles"), Cint, ())
    
    vx = zeros(npart)
    vy = zeros(npart)
    vz = zeros(npart)

    ccall(dlsym(lib, "sepGetVelocities"), Cvoid,
                (Ptr{Cdouble}, Ptr{Cdouble}, Ptr{Cdouble}), vx, vy, vz)
    
    return vx,vy,vz
end


function GetMasses()

    npart = ccall(dlsym(lib, "sepGetNumbParticles"), Cint, ())
    
    m = zeros(npart)
    
    ccall(dlsym(lib, "sepGetMasses"), Cvoid, (Ptr{Cdouble}), m)
    
    return m
end


function GetCharges()

    npart = ccall(dlsym(lib, "sepGetNumbParticles"), Cint, ())
    
    z = zeros(npart)
    
    ccall(dlsym(lib, "sepGetCharges"), Cvoid, (Ptr{Cdouble}), z)
    
    return z
end

#Set functions

function SetTypes(types)

    npart = ccall(dlsym(lib, "sepGetNumbParticles"), Cint, ())

    if ( length(types) != npart )
        error("Length of type arrey not correct")
    end
    
    ccall(dlsym(lib, "sepSetTypes"), Cvoid, (Ptr{Cchar}), types)
    
end

function SetExclusionRule(rule)

    ccall(dlsym(lib, "sepSetExclusionRule"), Cvoid, (Ptr{Cchar}), rule)
    
end

function SetOMP(nthreads)

    ccall(dlsym(lib, "sepSetOMP"), Cvoid, (Cint), nthreads)

end

end

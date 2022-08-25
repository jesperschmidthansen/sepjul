module sep

using Libdl

strpath="/home/jschmidt/software/sepjul/module/libseptojul.so"
lib = dlopen(strpath)

## Init block
function Init(xyzfile)

    ccall(dlsym(lib, "sepInit"), Cvoid, (Cstring,), xyzfile)

end

function (xyzfile)

    ccall(dlsym(lib, "sepInit"), Cvoid, (Cstring,), xyzfile)

end


function Close()

    ccall(dlsym(lib, "sepClear"), Cvoid, ())

end

function Reset()

    ccall(dlsym(lib, "sepReset"), Cvoid, ())

end

function ForceLJ(types, params)

    ccall(dlsym(lib, "sepForceLJ"), Cvoid, (Cstring, Ptr{Cdouble}), types, params)

end

function RelaxTemp(type, desiredTemp, tau)

    ccall(dlsym(lib, "sepRelaxTemp"), Cvoid, (Cchar, Cdouble, Cdouble), type, desiredTemp, tau)
 
end

function LeapFrog()

    ccall(dlsym(lib, "sepLeapFrog"), Cvoid, ())

end

function Save(filename)

    ccall(dlsym(lib, "sepSave"), Cvoid, (Cstring,), filename)

end

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

end

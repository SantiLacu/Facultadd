{Se desea administrar el stock de los productos de una tienda de electrodomésticos con varias sucursales en el país. 
Para ello se cuenta con un archivo maestro donde figuran todos los productos que comercializa. De cada producto se almacena la siguiente 
información: código de producto, nombre comercial, descripción, precio de venta, cantidad vendida, y mayor cantidad vendida en un mes. 
Mensualmente se genera un archivo detalle en cada sucursal en el que registran todas las ventas de productos. De cada venta se registra el 
código de producto y la cantidad de unidades vendidas. Mensualmente la empresa recibe un archivo detalle de cada sucursal (son 8 sucursales) 
y debe actualizar el archivo maestro. Se pide realizar un programa que realice la declaración de tipos e invoque un proceso que actualice el 
archivo maestro con los archivos detalle sabiendo que: a. Todos los archivos están ordenados por código de producto. b. Cada registro del 
archivo maestro puede ser actualizado por 0, 1 ó más registros de los archivos detalle. c. Los archivos detalle sólo contienen ventas de 
productos que están en el archivo maestro.  Además si la cantidad vendida en el mes actual supera a la mayor cantidad vendida en un mes previo,
se debe actualizar este dato y también se debe informar en pantalla el código del producto, nombre, mayor cantidad vendida hasta el mes 
anterior (la del archivo maestro) y cantidad vendida en el mes actual. Nota: deberá implementar el programa principal, todos los procedimientos
y los tipos de datos necesarios. 
}
const
    valorAlto=9999;
    str=50;
    cant_detalle=8;
type
    dato_maestro = record
                   cod_prod:integer;
                   nombre:string[str];
                   desc:string[str];
                   precio:double;
                   cant_v:integer;
                   mayor_cant:integer;
                   end;
    dato_detalle = record
                   cod_prod:integer;
                   cant_v:integer;
                   end;
    maestro = file of dato_maestro;
    detalle = file of dato_detalle;
    arDet = array [1..cant_detalle] of detalle;
    arDat = array [1..cant_detalle] of dato_detalle;
procedure leerD(var archivo:detalle; var dato:dato_detalle);
begin
    if (not EOF(archivo))then 
        read(archivo,dato)
    else
        dato.cod_prod:=valorAlto;
end;
procedure Minimo(var vecAr:arDet; var vecDat:arDat; var min: dato_detalle);
var
    i,pos:integer;
begin
    pos:=1;
    min:=vecDat[pos];
    for i:= 1 to cant_detalle do begin
        if(vecDat[i].cod_prod<min.cod_prod)then begin
            min:=vecDat[i];
            pos:=i;
        end;
    end;
    read(vecAr[pos],vecDat[pos]);
end;
procedure actualizarMaestro(var archivo_maestro:maestro;var vecAr:arDet; var vecDat:arDat);  
var
    min:dato_detalle;
    aux:dato_maestro;
    cantV:integer;
begin
    Reset(archivo_maestro);
    read(archivo_maestro,aux);
    Minimo(vecAr,vecDat,min);
    cantV:=0;
    while(min.cod_prod<>valorAlto)do begin          //Dice que todos los prod del detalle estan en el maestro,deberia ver igual q no se termine?
        while(aux.cod_prod<>min.cod_prod)do
            read(archivo_maestro,aux);
        while(aux.cod_prod=min.cod_prod)do begin
            cantV:=cantV+min.cant_v;
            Minimo(vecAr,vecDat,min);
        end;
        if(cantV>aux.mayor_cant)then begin
            writeln('codigo del producto: '+aux.cod_prod+' nombre: '+aux.nombre+' cantidad vendida el mes pasado: '+aux.mayor_cant+
                                ' cantidad vendia este mes: '+cantV);
            aux.mayor_cant:=cantV;
        end;
        seek(archivo_maestro,filePos(archivo_maestro)-1);
        aux.cant_v:=cantV;
        cantV:=0;
        write(archivo_maestro,aux);
    end;
end;
var
    archivo_maestro:maestro;
    vecAr:arDet;
    vecDat:arDat;
    i:integer;
begin
    Assign(archivo_maestro,'maestro.bin');
    for i := 1 to cant_detalle do begin
        Assign(vecAr[i],'datelle'+i+'.bin');
        reset(vecAr[i]);
        read(vecAr[i],vecDat[i]);    
    end;
    actualizarMaestro(archivo_maestro,vecAr,VecDat);
    close(archivo_maestro);
    for i := 1 to cant_detalle do
        close(vecAr[i]);
end.
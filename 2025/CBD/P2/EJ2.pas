//La municipalidad de la Plata, en pos de minimizar los efectos de posibles inundaciones, 
//construye acueductos que permitan canalizar rápidamente el agua de la ciudad hacia diferentes
//arroyos que circundan la misma. La construcción está dividida por zonas. Los arquitectos 
//encargados de las obras realizan recorridos diariamente y guardan información de la zona,
//fecha y cantidad de metros construidos. Cada arquitecto envía mensualmente un archivo que
//contiene la siguiente información: cod_zona, nombre de la zona, descripción de ubicación 
//geográfica, fecha, cantidad de metros construidos ese día. Se sabe que en la obra trabajan 
//15 arquitectos y que durante el mes van rotando de zona. Escriba un procedimiento que reciba
//los 15 archivos correspondiente y genere un archivo maestro indicando para 
//cada zona: cod_zona, nombre de zona y cantidad de metros construidos. 
//Además se deberá informar en un archivo de texto, para cada zona, la cantidad de metros 
//construidos indicando: cod_zona, nombre, ubicación y metros construidos.
//Nota: todos los archivos están ordenados por cod_zona. 
const
    str=50;
    valorAlto=9999;
    cantDetalles=15;
type
    datoD = record 
            cod_zona: integer;
            nombre_zona: string[str];
            desc: string[str];
            fecha: string[str];
            cant_metro: integer;
            end;
    datoM = record
            cod_zona: integer;
            nombre_zona: string[str];
            cant_metro: integer;
            end;
    maestro = file of datoM;
    detalle = file of datoD;
    vectorDetalle = array [1..cantDetalles] of detalle;
    vectorDatoD = array [1..cantDetalles] of datoD;

    procedure leerD (var archivo:detalle;var dato:datoD);
    begin
        if(not EOF(archivo))then
            read(archivo,dato)
        else
            dato.cod_zona:=valorAlto;
    end;
    procedure Minimo (var vectorDet:vectorDetalle; var vectorDato:vectorDatoD; var min:datoD);
    var
        pos,i:integer;
    begin
        pos:=1;
        min:=vectorDato[pos];
        for i:= 1 to cantDetalles  do begin
            if(min.cod_zona>vectorDato[i].cod_zona) then begin
                min:=vectorDato[i];
                pos:=i;
            end;
        end;
        leerD(vectorDet[pos],vectorDato[pos]);
    end;

    procedure crearMaestro(var vectorDet:vectorDetalle; var vectorDato:vectorDatoD; var archivoMaestro:maestro);
    var
        archivoT: Text;
        min:datoD;
        aux:datoM;
        desc,nom:string[str];
    begin
        Rewrite(archivoT);
        Rewrite(archivoMaestro);
        Minimo(vectorDet,vectorDato,min);
        aux.cod_zona:=min.cod_zona;
        aux.cant_metro:=0;
        desc:=min.desc;
        nom:=min.nombre_zona;
        while (min.cod_zona<>valorAlto) do begin
            while (aux.cod_zona=min.cod_zona) do begin
                aux.cant_metro:=aux.cant_metro+min.cant_metro;
                Minimo(vectorDet,vectorDato,min);
                end;
            writeln(archivoT,'cod_zona: '+aux.cod_zona+' nombre '+nom+' ubicacion: '+desc+' metros: '+aux.cant_metro);
            aux.cant_metro:=0;
            aux.cod_zona:=min.cod_zona;
            desc:=min.desc;
            nom:=min.nombre_zona;
        end;
        close(archivoMaestro);
        close(archivoT);
    end;

begin
end.
// Archivo: EjParcial.pas
// Generación de archivo maestro y reporte a partir de archivos detalle de ventas de autos
// Estilo y estructura según ejercicios de P2 y P3

const
  N = 20;
  str = 30;

type
  tDetalle = record
    anio: integer;
    mes: integer;
    dia: integer;
    codMarca: integer;
    codModelo: integer;
    nombreMarca: string[str];
    nombreModelo: string[str];
    color: string[str];
    dniCliente: string[10];
    monto: real;
  end;

  tMaestro = record
    anio: integer;
    mes: integer;
    totalMonto: real;
    totalVentas: integer;
  end;

  archivoDetalle = file of tDetalle;
  archivoMaestro = file of tMaestro;

procedure leerDetalle(var arch: archivoDetalle; var reg: tDetalle);
begin
  if not EOF(arch) then
    read(arch, reg)
  else
    reg.anio := 9999; // valor alto
end;

procedure minimo(var detalles: array of archivoDetalle; var regs: array of tDetalle; var min: tDetalle; var minIdx: integer);
var
  i: integer;
begin
  min.anio := 9999;
  minIdx := -1;
  for i := 1 to N do
    if (regs[i].anio < min.anio) or
       ((regs[i].anio = min.anio) and (regs[i].mes < min.mes)) or
       ((regs[i].anio = min.anio) and (regs[i].mes = min.mes) and (regs[i].dia < min.dia)) or
       ((regs[i].anio = min.anio) and (regs[i].mes = min.mes) and (regs[i].dia = min.dia) and (regs[i].codMarca < min.codMarca)) or
       ((regs[i].anio = min.anio) and (regs[i].mes = min.mes) and (regs[i].dia = min.dia) and (regs[i].codMarca = min.codMarca) and (regs[i].codModelo < min.codModelo)) then
    begin
      min := regs[i];
      minIdx := i;
    end;
  if minIdx <> -1 then
    leerDetalle(detalles[minIdx], regs[minIdx]);
end;

procedure actualizarMaestroYReporte(var maestro: archivoMaestro; var detalles: array of archivoDetalle);
var
  regs: array[1..N] of tDetalle;
  min: tDetalle;
  minIdx: integer;
  actualAnio, actualMes: integer;
  totalMonto: real;
  totalVentas: integer;
  regM: tMaestro;
  archTexto: text;
  actualMarca, actualModelo, actualDia: integer;
  nombreMarca, nombreModelo: string[str];
  ventasDia: integer;
  modeloMax, modeloMin: string[str];
  ventasMax, ventasMin: integer;
  i: integer;
  maxVentasDia, minVentasDia: integer;
begin
  for i := 1 to N do
    reset(detalles[i]);
  rewrite(maestro);
  for i := 1 to N do
    leerDetalle(detalles[i], regs[i]);
  assign(archTexto, 'reporte.txt');
  rewrite(archTexto);
  minimo(detalles, regs, min, minIdx);
  while min.anio <> 9999 do
  begin
    actualAnio := min.anio;
    actualMes := min.mes;
    totalMonto := 0;
    totalVentas := 0;
    while (min.anio = actualAnio) and (min.mes = actualMes) do
    begin
      actualMarca := min.codMarca;
      nombreMarca := min.nombreMarca;
      writeln(archTexto, 'Marca: ', nombreMarca);
      ventasMax := -1; ventasMin := 9999;
      modeloMax := ''; modeloMin := '';
      while (min.anio = actualAnio) and (min.mes = actualMes) and (min.codMarca = actualMarca) do
      begin
        actualModelo := min.codModelo;
        nombreModelo := min.nombreModelo;
        maxVentasDia := -1;
        minVentasDia := 9999;
        actualDia := min.dia;
        ventasDia := 0;
        // Un solo while para recorrer todos los registros de este modelo de la marca en el mes
        while (min.anio = actualAnio) and (min.mes = actualMes) and (min.codMarca = actualMarca) and (min.codModelo = actualModelo) do
        begin
          // Si cambia el día, actualizo max/min y reinicio contador
          if min.dia <> actualDia then
          begin
            if (ventasDia > maxVentasDia) or (maxVentasDia = -1) then
              maxVentasDia := ventasDia;
            if (ventasDia < minVentasDia) or (minVentasDia = 9999) then
              minVentasDia := ventasDia;
            actualDia := min.dia;
            ventasDia := 0;
          end;
          ventasDia := ventasDia + 1;
          totalVentas := totalVentas + 1;
          totalMonto := totalMonto + min.monto;
          minimo(detalles, regs, min, minIdx);
        end;
        // Al salir, actualizar por el último día
        if (ventasDia > maxVentasDia) or (maxVentasDia = -1) then
          maxVentasDia := ventasDia;
        if (ventasDia < minVentasDia) or (minVentasDia = 9999) then
          minVentasDia := ventasDia;
        // Actualizar max y min de modelo en un día
        if (maxVentasDia > ventasMax) or (ventasMax = -1) then
        begin
          ventasMax := maxVentasDia;
          modeloMax := nombreModelo;
        end;
        if (minVentasDia < ventasMin) or (ventasMin = 9999) then
        begin
          ventasMin := minVentasDia;
          modeloMin := nombreModelo;
        end;
      end;
      writeln(archTexto, 'Modelo más vendido: ', modeloMax, ' (', ventasMax, ')');
      writeln(archTexto, 'Modelo menos vendido: ', modeloMin, ' (', ventasMin, ')');
      writeln(archTexto, '');
    end;
    regM.anio := actualAnio;
    regM.mes := actualMes;
    regM.totalMonto := totalMonto;
    regM.totalVentas := totalVentas;
    write(maestro, regM);
  end;
  for i := 1 to N do
    close(detalles[i]);
  close(maestro);
  close(archTexto);
end;

var
  detalles: array[1..N] of archivoDetalle;
  maestro: archivoMaestro;
  i: integer;
begin
  for i := 1 to N do
    assign(detalles[i], 'detalle' + IntToStr(i) + '.dat');
  assign(maestro, 'maestro.dat');
  actualizarMaestroYReporte(maestro, detalles);
end.

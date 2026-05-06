#include "tecnico.hpp"
#include "motorlib/util.h"
#include <iostream>
#include <queue>
#include <set>
#include <map>

using namespace std;

// =========================================================================
// ÁREA DE IMPLEMENTACIÓN DEL ESTUDIANTE
// =========================================================================

Action ComportamientoTecnico::think(Sensores sensores) {
  Action accion = IDLE;


  // Decisión del agente según el nivel
  switch (sensores.nivel) {
    case 0: accion = ComportamientoTecnicoNivel_0(sensores); break;
    case 1: accion = ComportamientoTecnicoNivel_1(sensores); break;
    case 2: accion = ComportamientoTecnicoNivel_2(sensores); break;
    case 3: accion = ComportamientoTecnicoNivel_3(sensores); break;
    case 4: accion = ComportamientoTecnicoNivel_4(sensores); break;
    case 5: accion = ComportamientoTecnicoNivel_5(sensores); break;
    case 6: accion = ComportamientoTecnicoNivel_6(sensores); break;
  }

  return accion;
}

/**
 * @brief Determina si casilla viable por altura
 * @param casilla tipo de terreno
 * @param dif diferencia de altura entre casillas
 * @return 'P' si no es accesible por altura y casilla en otro caso
 */
char ViablePorAlturaT (char casilla, int dif){
  if (abs(dif) <= 1)
    return casilla;
  else
    return 'P';
}

/**
 * @brief Determina la mejor opción entre las 3 casillas que tiene delante
 * @param i terreno que hay en la posición 1 de superficie (45 izq)
 * @param c terreno que hay en la posición 2 de superficie (justo delante)
 * @param d terreno que hay en la posición 3 de superficie (45 dch)
 * @return 2 si es mejor WALK, 1 para TURN_SL y 3 para TURN_SR. 0 no hay nada interesante
 */
int VeoCasillaInteresanteT (char i, char c, char d){
  if (c == 'U') return 2;
  else if (i == 'U') return 1;
  else if (d == 'U') return 3;
  if (c == 'X') return 2;
  else if (i == 'X') return 1;
  else if (d == 'X') return 3;
  if (c == 'C') return 2;
  else if (i == 'C') return 1;
  else if (d == 'C') return 3;
  else return 0;
}

// Niveles del técnico
Action ComportamientoTecnico::ComportamientoTecnicoNivel_0(Sensores sensores) {
  Action accion = IDLE;
  // El comportamiento de seguir un camino hasta encontrar una planta de T. Residuos
  // Poner el valor de los sensores de visión sobre los mapas.
  ActualizarMapa(sensores);

  // Actualización de variables de estado
  if (sensores.superficie[0] == 'D') tiene_zapatillas = true;
  //Definición del comportamiento
  if (sensores.superficie[0] == 'U'){ // Llegué a una 'U'
    return IDLE;
  }

  char i = ViablePorAlturaT(sensores.superficie[1], sensores.cota[1]-sensores.cota[0]);
  char c = ViablePorAlturaT(sensores.superficie[2], sensores.cota[2]-sensores.cota[0]);
  char d = ViablePorAlturaT(sensores.superficie[3], sensores.cota[3]-sensores.cota[0]);

  if(sensores.agentes[1] != '_'){
    i = 'P';
  }
  if(sensores.agentes[2] != '_'){
    c = 'P';
  }
  if(sensores.agentes[3] != '_'){
    d = 'P';
  }
  
  int pos = VeoCasillaInteresanteT(i, c, d);

  switch (pos)
  {
  case 2:
    if(aleatorio(100) < 95){
      accion = WALK;
    }else if(i == 'C' && aleatorio(10) < 6){
      accion = TURN_SL;
    }else if(d == 'C'){
      accion = TURN_SR;
    }else{
      if(aleatorio(10) < 5){ 
        accion = TURN_SL;
      }else{
        accion = TURN_SR;
      }  
    }
    break;
  case 1:
    if(aleatorio(100) < 95){
      accion = TURN_SL;
    }else{
      accion = TURN_SR;
    }
    break;
  case 3:
    if(aleatorio(100) < 95){
      accion = TURN_SR;
    }else{
      accion = TURN_SL;
    }
    break;
  default:
   if(aleatorio(10) < 6){ 
      accion = TURN_SL;
    }else{
      accion = TURN_SR;
    }
    break;
  }

  // Devolver la siguiente accion a hacer
  last_action = accion;
  return accion;
}

/**
 * @brief Comprueba si una celda es de tipo camino transitable.
 * @param c Carácter que representa el tipo de superficie.
 * @return true si es camino ('C'), zapatillas ('D') o meta ('U').
 */
bool ComportamientoTecnico::es_camino(unsigned char c) const {
  return (c == 'C' || c == 'D' || c == 'U');
}

/**
 * @brief Comprueba si una celda es de tipo sendero transitable.
 * @param c Carácter que representa el tipo de superficie.
 * @return true si es camino ('S').
 */
bool ComportamientoTecnico::es_sendero(unsigned char c) const
{
  return (c == 'S');
}

/**
 * @brief Comportamiento reactivo del técnico para el Nivel 1.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoTecnico::ComportamientoTecnicoNivel_1(Sensores sensores) {
Action accion = IDLE;
  // El comportamiento de seguir un camino hasta encontrar una planta de T. Residuos
  // Poner el valor de los sensores de visión sobre los mapas.
  ActualizarMapa(sensores);

  char i = ViablePorAlturaT(sensores.superficie[1], sensores.cota[1]-sensores.cota[0]);
  char c = ViablePorAlturaT(sensores.superficie[2], sensores.cota[2]-sensores.cota[0]);
  char d = ViablePorAlturaT(sensores.superficie[3], sensores.cota[3]-sensores.cota[0]);

  bool esCamSenDelante = es_camino(c) || es_sendero(c);
  bool esCamSenIzq = es_camino(i) || es_sendero(i);
  bool esCamSenDer = es_camino(d) || es_sendero(d);

  if(esCamSenDelante && sensores.agentes[2] == '_' && aleatorio(10) < 7){
    accion = WALK;
  }else if(esCamSenIzq && sensores.agentes[1] == '_'){
    accion = TURN_SL;
  }else if(esCamSenDer && sensores.agentes[3] == '_'){
    accion = TURN_SR;
  }else{
    if(aleatorio(10) < 6){ 
      accion = TURN_SL;
    }else{
      accion = TURN_SR;
    }  
  }

  // Devolver la siguiente accion a hacer
  last_action = accion;
  return accion;
}

list<Action> AvanzaASaltosDeCaballo(){
  list<Action> secuencia;
  secuencia.push_back(WALK);
  secuencia.push_back(WALK);
  secuencia.push_back(TURN_SR);
  secuencia.push_back(TURN_SR);
  secuencia.push_back(WALK);
  return secuencia;
}

/**
 * @brief Comportamiento del técnico para el Nivel E.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoTecnico::ComportamientoTecnicoNivel_E(Sensores sensores) {
  Action accion = IDLE;
  if (!hayPlan){
    // Invocar al método de búsqueda
    EstadoT inicio, fin;
    inicio.site.f = sensores.posF;
    inicio.site.c = sensores.posC;
    inicio.site.brujula = sensores.rumbo;
    inicio.zapatillas = tiene_zapatillas;
    fin.site.f = sensores.BelPosF;
    fin.site.c = sensores.BelPosC;
    plan = B_Anchura_V2(inicio, fin, mapaResultado, mapaCotas);
    VisualizaPlan(inicio.site,plan);
    hayPlan = plan.size() != 0 ;
  }
  if (hayPlan and plan.size()>0){
    accion = plan.front();
    plan.pop_front();
  }
  if (plan.size()== 0){
    hayPlan = false;
  }
  return accion;
}

EstadoT NextCasillaTécnico(const EstadoT &st){
  EstadoT siguiente = st;
  switch (st.site.brujula)
  {
  case norte:
    siguiente.site.f = st.site.f - 1;
    break;
  case noreste:
    siguiente.site.f = st.site.f - 1;
    siguiente.site.c = st.site.c + 1;
    break;
  case este:
    siguiente.site.c = st.site.c + 1;
    break;
  case sureste:
    siguiente.site.f = st.site.f + 1;
    siguiente.site.c = st.site.c + 1;
    break;
  case sur:
    siguiente.site.f = st.site.f + 1;
    break;
  case suroeste:
    siguiente.site.f = st.site.f + 1;
    siguiente.site.c = st.site.c - 1;
    break;
  case oeste:
    siguiente.site.c = st.site.c - 1;
    break;
  case noroeste:
    siguiente.site.f = st.site.f - 1;
    siguiente.site.c = st.site.c - 1;
  }
  return siguiente;
} 

bool CasillaAccesibleTécnico(const EstadoT &st, const vector<vector<unsigned char>> &terreno, const
                              vector<vector<unsigned char>> &altura){
  EstadoT next = NextCasillaTécnico(st);
  bool check1 = false, check2 = false, check3 = false;
  check1 = terreno[next.site.f][next.site.c] != 'P' and terreno[next.site.f][next.site.c] != 'M';
  check2 = terreno[next.site.f][next.site.c] != 'B' or (terreno[next.site.f][next.site.c] == 'B' and st.zapatillas);
  check3 = abs(altura[next.site.f][next.site.c] - altura[st.site.f][st.site.c]) <= 1;
  return check1 and check2 and check3;
} 

EstadoT applyT(Action accion, const EstadoT & st, const vector<vector<unsigned char>> &terreno, const
                vector<vector<unsigned char>> &altura){
  EstadoT next = st;
  switch(accion){
  case WALK:
    if (CasillaAccesibleTécnico(st,terreno,altura)){
      next = NextCasillaTécnico(st);
    }
    break;
  case TURN_SR:
    next.site.brujula = (Orientacion) ((next.site.brujula+1)%8);
    break;
  case TURN_SL:
    next.site.brujula = (Orientacion) ((next.site.brujula+7)%8);
    break;
  }
  return next;
}

bool Find (const NodoT & st, const list<NodoT> &lista){
  auto it = lista.begin();
  while (it != lista.end() and !((*it) == st)){
    it++;
  }
  return (it != lista.end());
}

/**
 * @brief Primera aproximación a la búsqueda en anchura
 * 
 * @param inicio Estado Inicial de la búsqueda
 * @param final Estado final de la búsqueda
 * @param terreno Matriz que contiene la información del terreno
 * @param altura Matriz que contiene la altura del mapa
 * 
 * @return La secuencia de acciones para llegar al estado final
 * @note Devuelve un plan vacío si no es posible encontrar un plan válido
 */
list<Action> ComportamientoTecnico::B_Anchura(const EstadoT &inicio, const EstadoT &final,
                          const vector<vector<unsigned char>> &terreno, 
                          const vector<vector<unsigned char>> &altura){
  NodoT current_node;
  list<NodoT> frontier;
  list<NodoT> explored;
  list<Action> path;

  current_node.estado = inicio;
  frontier.push_back(current_node);
  bool SolutionFound = (current_node.estado.site.f == final.site.f and current_node.estado.site.c == final.site.c);
  while (!SolutionFound and !frontier.empty()){
    frontier.pop_front();
    explored.push_back(current_node);

    // Compruebo si estoy en una casilla que de las zapatillas
    if (terreno[current_node.estado.site.f][current_node.estado.site.c] == 'D'){
      current_node.estado.zapatillas = true;
    }

    // Genero el hijo resultante de aplicar la acción WALK
    NodoT child_Walk = current_node;
    child_Walk.estado = applyT(WALK, current_node.estado, terreno, altura);
    if (child_Walk.estado.site.f == final.site.f and child_Walk.estado.site.c == final.site.c){
      // El hijo generado es solucion
      child_Walk.secuencia.push_back(WALK);
      current_node = child_Walk;
      SolutionFound = true;
    }
    else if (!Find(child_Walk, frontier) and !Find(child_Walk, explored)){
      // Se mete en la lista de frontier después de añadir a secuencia la acción
      child_Walk.secuencia.push_back(WALK);
      frontier.push_back(child_Walk);
    }

    if (!SolutionFound){
      // El hijo resultante de aplicar la acción TURN_SR
      NodoT child_TurnSR = current_node;
      child_TurnSR.estado = applyT(TURN_SR, current_node.estado, terreno, altura);
      if (!Find(child_TurnSR, frontier) and !Find(child_TurnSR, explored)){
        child_TurnSR.secuencia.push_back(TURN_SR);
        frontier.push_back(child_TurnSR);
      }

      // El hijo resultante de aplcar de aplicar la accion TURN_SL
      NodoT child_TurnSL = current_node;
      child_TurnSL.estado = applyT(TURN_SL, current_node.estado, terreno, altura);
      if (!Find(child_TurnSL, frontier) and !Find(child_TurnSL, explored)){
        child_TurnSL.secuencia.push_back(TURN_SL);
        frontier.push_back(child_TurnSL);
      }
    }

    // Paso a evaluar el siguiente nodo en la lista "frontier"
    if (!SolutionFound and !frontier.empty()){
      current_node = frontier.front();
      SolutionFound = (current_node.estado.site.f == final.site.f and current_node.estado.site.c == final.site.c); 
    }
  }
  // Devuelvo el camino encontrado
  if (SolutionFound)
    path = current_node.secuencia;
  
  return path;
}

list<Action> ComportamientoTecnico::B_Anchura_V2(const EstadoT &inicio, const EstadoT &final,
                          const vector<vector<unsigned char>> &terreno, 
                          const vector<vector<unsigned char>> &altura){
  NodoT current_node;
  list<NodoT> frontier;
  set<NodoT> explored;
  list<Action> path;

  current_node.estado = inicio;
  frontier.push_back(current_node);
  bool SolutionFound = (current_node.estado.site.f == final.site.f and current_node.estado.site.c == final.site.c);
  while (!SolutionFound and !frontier.empty()){
    frontier.pop_front();
    explored.insert(current_node);

    // Compruebo si estoy en una casilla que da las zapatillas
    if (terreno[current_node.estado.site.f][current_node.estado.site.c] == 'D'){
      current_node.estado.zapatillas = true;
    }

    // Genero el hijo resultante de aplicar la acción WALK
    NodoT child_Walk = current_node;
    child_Walk.estado = applyT(WALK, current_node.estado, terreno, altura);
    if (child_Walk.estado.site.f == final.site.f and child_Walk.estado.site.c == final.site.c){
      // El hijo generado es solucion
      child_Walk.secuencia.push_back(WALK);
      current_node = child_Walk;
      SolutionFound = true;
    }
    else if (explored.find(child_Walk) == explored.end()){
      // Se mete en la lista de frontier después de añadir a secuencia la acción
      child_Walk.secuencia.push_back(WALK);
      frontier.push_back(child_Walk);
    }

    if (!SolutionFound){
      // El hijo resultante de aplicar la acción TURN_SR
      NodoT child_TurnSR = current_node;
      child_TurnSR.estado = applyT(TURN_SR, current_node.estado, terreno, altura);
      if (explored.find(child_TurnSR) == explored.end()){
        child_TurnSR.secuencia.push_back(TURN_SR);
        frontier.push_back(child_TurnSR);
      }

      // El hijo resultante de aplcar de aplicar la accion TURN_SL
      NodoT child_TurnSL = current_node;
      child_TurnSL.estado = applyT(TURN_SL, current_node.estado, terreno, altura);
      if (explored.find(child_TurnSL) == explored.end()){
        child_TurnSL.secuencia.push_back(TURN_SL);
        frontier.push_back(child_TurnSL);
      }
    }

    // Paso a evaluar el siguiente nodo en la lista "frontier"
    if (!SolutionFound and !frontier.empty()){
      current_node = frontier.front();
      while (explored.find(current_node) != explored.end() and !frontier.empty()){
        frontier.pop_front();
        current_node = frontier.front();
      }
    }
  }

  // Devuelvo el camino encontrado.
  if (SolutionFound)
    path = current_node.secuencia;
  
  return path;
}

/**
 * @brief Comportamiento del técnico para el Nivel 2.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoTecnico::ComportamientoTecnicoNivel_2(Sensores sensores) {
  bool veoIngeniero = false;
  for(int i = 1; i < 16; i++){
    if(sensores.agentes[i] != '_'){
        veoIngeniero = true;
        break;
    }
  }
  if(veoIngeniero){
    char c = ViablePorAlturaT(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
    bool frenteLibre = (c != 'P' && c != 'M' && c != 'B' && sensores.agentes[2] == '_');
    
    if(frenteLibre){
      return WALK;
    }else{
      return TURN_SR;
    }
  }
  return IDLE;
}

int CosteBateriaTecnico(Action accion, const EstadoT &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
  int coste = 0;
  char casilla_actual = terreno[st.site.f][st.site.c];
    
  if(accion == WALK){
    if(casilla_actual == 'A') coste = 60;
    else if(casilla_actual == 'H') coste = 6;
    else if(casilla_actual == 'S') coste = 3;
    else coste = 1;

    if(casilla_actual == 'A' || casilla_actual == 'H' || casilla_actual == 'S'){
      EstadoT next = NextCasillaTécnico(st);
      int dif_altura = altura[next.site.f][next.site.c] - altura[st.site.f][st.site.c];
      if(dif_altura > 0) coste += 5;
      else if(dif_altura < 0) coste -= 2;
    }
  }else if(accion == TURN_SL || accion == TURN_SR){
    if(casilla_actual == 'A') coste = 5;
    else if(casilla_actual == 'H') coste = 2;
    else if(casilla_actual == 'S') coste = 1;
    else coste = 1;
  }
  return coste;
}

int HeuristicaT(const EstadoT &st, const EstadoT &final){
    return max(abs(st.site.f - final.site.f), abs(st.site.c - final.site.c));
}

list<Action> ComportamientoTecnico::algoritmoA(const EstadoT &inicio, const EstadoT &final,
                          const vector<vector<unsigned char>> &terreno, 
                          const vector<vector<unsigned char>> &altura){
  NodoT current_node;
  priority_queue<NodoT, vector<NodoT>, ComparadorNodoT> frontier;
    
  map<EstadoT, int> min_g; 
  current_node.estado = inicio;
  if(terreno[inicio.site.f][inicio.site.c] == 'D'){
    current_node.estado.zapatillas = true;
  }
    
  current_node.g = 0;
  current_node.h = HeuristicaT(current_node.estado, final); 
  frontier.push(current_node);
  min_g[current_node.estado] = 0;
  bool SolutionFound = false;

  while(!frontier.empty()){
    current_node = frontier.top();
    frontier.pop();

    if(current_node.estado.site.f == final.site.f && current_node.estado.site.c == final.site.c){
      SolutionFound = true;
      break;
    }

    if(current_node.g > min_g[current_node.estado]){
      continue;
    }

    Action acciones[] = {WALK, TURN_SR, TURN_SL};
    for(Action accion : acciones){
            
      if(accion == WALK && !CasillaAccesibleTécnico(current_node.estado, terreno, altura)){
        continue; 
      }

      NodoT child = current_node;
      child.estado = applyT(accion, current_node.estado, terreno, altura);
            
      if(accion == WALK && terreno[child.estado.site.f][child.estado.site.c] == 'D'){
        child.estado.zapatillas = true;
      }

      int coste_mov = CosteBateriaTecnico(accion, current_node.estado, terreno, altura);
      child.g = current_node.g + coste_mov;
      child.h = HeuristicaT(current_node.estado, final); 
      child.secuencia.push_back(accion);

      if(min_g.find(child.estado) == min_g.end() || child.g < min_g[child.estado]){
        min_g[child.estado] = child.g;
        frontier.push(child);
      }
    }
  }
  if(SolutionFound){
    return current_node.secuencia;
  }
  return list<Action>();
}

/**
 * @brief Comportamiento del técnico para el Nivel 3.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoTecnico::ComportamientoTecnicoNivel_3(Sensores sensores) {
  Action accion = IDLE;

  if(!hayPlan){
    EstadoT inicio, fin;
    inicio.site.f = sensores.posF;
    inicio.site.c = sensores.posC;
    inicio.site.brujula = sensores.rumbo;
    inicio.zapatillas = tiene_zapatillas;
        
    fin.site.f = sensores.BelPosF;
    fin.site.c = sensores.BelPosC;
    plan = algoritmoA(inicio, fin, mapaResultado, mapaCotas);
    VisualizaPlan(inicio.site, plan);
    hayPlan = plan.size() > 0;
  }

  if(hayPlan && plan.size() > 0){
    accion = plan.front();
    plan.pop_front();
  }
  if(plan.size() == 0){
    hayPlan = false;
  }
  return accion;
}

/**
 * @brief Comportamiento del técnico para el Nivel 4.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoTecnico::ComportamientoTecnicoNivel_4(Sensores sensores) {
  return IDLE;
}

/**
 * @brief Comportamiento del técnico para el Nivel 5.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoTecnico::ComportamientoTecnicoNivel_5(Sensores sensores) {
  Action accion = IDLE;

  if(sensores.choque || sensores.reset) hayPlan = false;

  if(sensores.venpaca && (sensores.GotoF != f_obj || sensores.GotoC != c_obj)){
    f_obj = sensores.GotoF;
    c_obj = sensores.GotoC;
    estadoNivel5 = 1; 
    hayPlan = false; // Descartamos plan anterior
  }

  switch(estadoNivel5){
    case 0: // Espera la llamada COME
    {
      bool veoIngeniero = false;  // Comprobamos si viene ingeniero
      for(int i = 1; i < 16; i++){
        if(sensores.agentes[i] != '_'){
          veoIngeniero = true;
          break;
        }
      }
      if(veoIngeniero){
        accion = ComportamientoTecnicoNivel_2(sensores);
      }else{
        accion = IDLE;
      }
      break;
    }

    case 1: // Coloca tuberias o se ordena
      if(sensores.posF == f_obj && sensores.posC == c_obj){
        hasEstadoCOME = true;
        if(sensores.enfrente){
          accion = INSTALL; 
          estadoNivel5 = 0;
        }else if(sensores.agentes[2] != '_'){ 
          accion = IDLE; // Ingeniero operando terreno
        }else{
          accion = TURN_SR; // Busca a ingeniero
        }
      }else{
        if(!hayPlan){
          EstadoT inicio, fin;
          inicio.site.f = sensores.posF; 
          inicio.site.c = sensores.posC;
          inicio.site.brujula = sensores.rumbo; 
          inicio.zapatillas = tiene_zapatillas;
          fin.site.f = f_obj; 
          fin.site.c = c_obj;
          plan = algoritmoA(inicio, fin, mapaResultado, mapaCotas);
          hayPlan = (plan.size() > 0);
        }
        
        if(hayPlan && !plan.empty()){
          if(plan.front() == WALK && sensores.agentes[2] != '_' && !hasEstadoCOME){
            plan.clear();
            
            bool derLibre = (sensores.superficie[3] != 'P' && sensores.superficie[3] != 'M' && (sensores.superficie[3] != 'B' || tiene_zapatillas) && sensores.agentes[3] == '_');
            bool izqLibre = (sensores.superficie[1] != 'P' && sensores.superficie[1] != 'M' && (sensores.superficie[3] != 'B' || tiene_zapatillas) && sensores.agentes[1] == '_');
            
            if(derLibre){
              plan.push_back(TURN_SR);
              plan.push_back(WALK);
            }else if (izqLibre){
              plan.push_back(TURN_SL);
              plan.push_back(WALK);
            }else{
              plan.push_back(TURN_SR);
            }
            accion = plan.front();
            plan.pop_front();
          }else{
            accion = plan.front(); 
            plan.pop_front();
            if(plan.empty()) hayPlan = false;
          }
        }else{
          hayPlan = false; 
          accion = TURN_SR; 
        }
      }
      break;
  }
  return accion;
}

/**
 * @brief Comportamiento del técnico para el Nivel 6.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoTecnico::ComportamientoTecnicoNivel_6(Sensores sensores) {
  return IDLE;
}




// =========================================================================
// FUNCIONES PROPORCIONADAS
// =========================================================================

/**
 * @brief Actualiza el mapaResultado y mapaCotas con la información de los sensores.
 * @param sensores Datos actuales de los sensores.
 */
void ComportamientoTecnico::ActualizarMapa(Sensores sensores) {
  mapaResultado[sensores.posF][sensores.posC] = sensores.superficie[0];
  mapaCotas[sensores.posF][sensores.posC] = sensores.cota[0];

  int pos = 1;
  switch (sensores.rumbo) {
    case norte:
      for (int j = 1; j < 4; j++)
        for (int i = -j; i <= j; i++) {
          mapaResultado[sensores.posF - j][sensores.posC + i] = sensores.superficie[pos];
          mapaCotas[sensores.posF - j][sensores.posC + i] = sensores.cota[pos++];
        }
      break;
    case noreste:
      mapaResultado[sensores.posF - 1][sensores.posC] = sensores.superficie[1];
      mapaCotas[sensores.posF - 1][sensores.posC] = sensores.cota[1];
      mapaResultado[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[2];
      mapaCotas[sensores.posF - 1][sensores.posC + 1] = sensores.cota[2];
      mapaResultado[sensores.posF][sensores.posC + 1] = sensores.superficie[3];
      mapaCotas[sensores.posF][sensores.posC + 1] = sensores.cota[3];
      mapaResultado[sensores.posF - 2][sensores.posC] = sensores.superficie[4];
      mapaCotas[sensores.posF - 2][sensores.posC] = sensores.cota[4];
      mapaResultado[sensores.posF - 2][sensores.posC + 1] = sensores.superficie[5];
      mapaCotas[sensores.posF - 2][sensores.posC + 1] = sensores.cota[5];
      mapaResultado[sensores.posF - 2][sensores.posC + 2] = sensores.superficie[6];
      mapaCotas[sensores.posF - 2][sensores.posC + 2] = sensores.cota[6];
      mapaResultado[sensores.posF - 1][sensores.posC + 2] = sensores.superficie[7];
      mapaCotas[sensores.posF - 1][sensores.posC + 2] = sensores.cota[7];
      mapaResultado[sensores.posF][sensores.posC + 2] = sensores.superficie[8];
      mapaCotas[sensores.posF][sensores.posC + 2] = sensores.cota[8];
      mapaResultado[sensores.posF - 3][sensores.posC] = sensores.superficie[9];
      mapaCotas[sensores.posF - 3][sensores.posC] = sensores.cota[9];
      mapaResultado[sensores.posF - 3][sensores.posC + 1] = sensores.superficie[10];
      mapaCotas[sensores.posF - 3][sensores.posC + 1] = sensores.cota[10];
      mapaResultado[sensores.posF - 3][sensores.posC + 2] = sensores.superficie[11];
      mapaCotas[sensores.posF - 3][sensores.posC + 2] = sensores.cota[11];
      mapaResultado[sensores.posF - 3][sensores.posC + 3] = sensores.superficie[12];
      mapaCotas[sensores.posF - 3][sensores.posC + 3] = sensores.cota[12];
      mapaResultado[sensores.posF - 2][sensores.posC + 3] = sensores.superficie[13];
      mapaCotas[sensores.posF - 2][sensores.posC + 3] = sensores.cota[13];
      mapaResultado[sensores.posF - 1][sensores.posC + 3] = sensores.superficie[14];
      mapaCotas[sensores.posF - 1][sensores.posC + 3] = sensores.cota[14];
      mapaResultado[sensores.posF][sensores.posC + 3] = sensores.superficie[15];
      mapaCotas[sensores.posF][sensores.posC + 3] = sensores.cota[15];
      break;
    case este:
      for (int j = 1; j < 4; j++)
        for (int i = -j; i <= j; i++) {
          mapaResultado[sensores.posF + i][sensores.posC + j] = sensores.superficie[pos];
          mapaCotas[sensores.posF + i][sensores.posC + j] = sensores.cota[pos++];
        }
      break;
    case sureste:
      mapaResultado[sensores.posF][sensores.posC + 1] = sensores.superficie[1];
      mapaCotas[sensores.posF][sensores.posC + 1] = sensores.cota[1];
      mapaResultado[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[2];
      mapaCotas[sensores.posF + 1][sensores.posC + 1] = sensores.cota[2];
      mapaResultado[sensores.posF + 1][sensores.posC] = sensores.superficie[3];
      mapaCotas[sensores.posF + 1][sensores.posC] = sensores.cota[3];
      mapaResultado[sensores.posF][sensores.posC + 2] = sensores.superficie[4];
      mapaCotas[sensores.posF][sensores.posC + 2] = sensores.cota[4];
      mapaResultado[sensores.posF + 1][sensores.posC + 2] = sensores.superficie[5];
      mapaCotas[sensores.posF + 1][sensores.posC + 2] = sensores.cota[5];
      mapaResultado[sensores.posF + 2][sensores.posC + 2] = sensores.superficie[6];
      mapaCotas[sensores.posF + 2][sensores.posC + 2] = sensores.cota[6];
      mapaResultado[sensores.posF + 2][sensores.posC + 1] = sensores.superficie[7];
      mapaCotas[sensores.posF + 2][sensores.posC + 1] = sensores.cota[7];
      mapaResultado[sensores.posF + 2][sensores.posC] = sensores.superficie[8];
      mapaCotas[sensores.posF + 2][sensores.posC] = sensores.cota[8];
      mapaResultado[sensores.posF][sensores.posC + 3] = sensores.superficie[9];
      mapaCotas[sensores.posF][sensores.posC + 3] = sensores.cota[9];
      mapaResultado[sensores.posF + 1][sensores.posC + 3] = sensores.superficie[10];
      mapaCotas[sensores.posF + 1][sensores.posC + 3] = sensores.cota[10];
      mapaResultado[sensores.posF + 2][sensores.posC + 3] = sensores.superficie[11];
      mapaCotas[sensores.posF + 2][sensores.posC + 3] = sensores.cota[11];
      mapaResultado[sensores.posF + 3][sensores.posC + 3] = sensores.superficie[12];
      mapaCotas[sensores.posF + 3][sensores.posC + 3] = sensores.cota[12];
      mapaResultado[sensores.posF + 3][sensores.posC + 2] = sensores.superficie[13];
      mapaCotas[sensores.posF + 3][sensores.posC + 2] = sensores.cota[13];
      mapaResultado[sensores.posF + 3][sensores.posC + 1] = sensores.superficie[14];
      mapaCotas[sensores.posF + 3][sensores.posC + 1] = sensores.cota[14];
      mapaResultado[sensores.posF + 3][sensores.posC] = sensores.superficie[15];
      mapaCotas[sensores.posF + 3][sensores.posC] = sensores.cota[15];
      break;
    case sur:
      for (int j = 1; j < 4; j++)
        for (int i = -j; i <= j; i++) {
          mapaResultado[sensores.posF + j][sensores.posC - i] = sensores.superficie[pos];
          mapaCotas[sensores.posF + j][sensores.posC - i] = sensores.cota[pos++];
        }
      break;
    case suroeste:
      mapaResultado[sensores.posF + 1][sensores.posC] = sensores.superficie[1];
      mapaCotas[sensores.posF + 1][sensores.posC] = sensores.cota[1];
      mapaResultado[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[2];
      mapaCotas[sensores.posF + 1][sensores.posC - 1] = sensores.cota[2];
      mapaResultado[sensores.posF][sensores.posC - 1] = sensores.superficie[3];
      mapaCotas[sensores.posF][sensores.posC - 1] = sensores.cota[3];
      mapaResultado[sensores.posF + 2][sensores.posC] = sensores.superficie[4];
      mapaCotas[sensores.posF + 2][sensores.posC] = sensores.cota[4];
      mapaResultado[sensores.posF + 2][sensores.posC - 1] = sensores.superficie[5];
      mapaCotas[sensores.posF + 2][sensores.posC - 1] = sensores.cota[5];
      mapaResultado[sensores.posF + 2][sensores.posC - 2] = sensores.superficie[6];
      mapaCotas[sensores.posF + 2][sensores.posC - 2] = sensores.cota[6];
      mapaResultado[sensores.posF + 1][sensores.posC - 2] = sensores.superficie[7];
      mapaCotas[sensores.posF + 1][sensores.posC - 2] = sensores.cota[7];
      mapaResultado[sensores.posF][sensores.posC - 2] = sensores.superficie[8];
      mapaCotas[sensores.posF][sensores.posC - 2] = sensores.cota[8];
      mapaResultado[sensores.posF + 3][sensores.posC] = sensores.superficie[9];
      mapaCotas[sensores.posF + 3][sensores.posC] = sensores.cota[9];
      mapaResultado[sensores.posF + 3][sensores.posC - 1] = sensores.superficie[10];
      mapaCotas[sensores.posF + 3][sensores.posC - 1] = sensores.cota[10];
      mapaResultado[sensores.posF + 3][sensores.posC - 2] = sensores.superficie[11];
      mapaCotas[sensores.posF + 3][sensores.posC - 2] = sensores.cota[11];
      mapaResultado[sensores.posF + 3][sensores.posC - 3] = sensores.superficie[12];
      mapaCotas[sensores.posF + 3][sensores.posC - 3] = sensores.cota[12];
      mapaResultado[sensores.posF + 2][sensores.posC - 3] = sensores.superficie[13];
      mapaCotas[sensores.posF + 2][sensores.posC - 3] = sensores.cota[13];
      mapaResultado[sensores.posF + 1][sensores.posC - 3] = sensores.superficie[14];
      mapaCotas[sensores.posF + 1][sensores.posC - 3] = sensores.cota[14];
      mapaResultado[sensores.posF][sensores.posC - 3] = sensores.superficie[15];
      mapaCotas[sensores.posF][sensores.posC - 3] = sensores.cota[15];
      break;
    case oeste:
      for (int j = 1; j < 4; j++)
        for (int i = -j; i <= j; i++) {
          mapaResultado[sensores.posF - i][sensores.posC - j] = sensores.superficie[pos];
          mapaCotas[sensores.posF - i][sensores.posC - j] = sensores.cota[pos++];
        }
      break;
    case noroeste:
      mapaResultado[sensores.posF][sensores.posC - 1] = sensores.superficie[1];
      mapaCotas[sensores.posF][sensores.posC - 1] = sensores.cota[1];
      mapaResultado[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[2];
      mapaCotas[sensores.posF - 1][sensores.posC - 1] = sensores.cota[2];
      mapaResultado[sensores.posF - 1][sensores.posC] = sensores.superficie[3];
      mapaCotas[sensores.posF - 1][sensores.posC] = sensores.cota[3];
      mapaResultado[sensores.posF][sensores.posC - 2] = sensores.superficie[4];
      mapaCotas[sensores.posF][sensores.posC - 2] = sensores.cota[4];
      mapaResultado[sensores.posF - 1][sensores.posC - 2] = sensores.superficie[5];
      mapaCotas[sensores.posF - 1][sensores.posC - 2] = sensores.cota[5];
      mapaResultado[sensores.posF - 2][sensores.posC - 2] = sensores.superficie[6];
      mapaCotas[sensores.posF - 2][sensores.posC - 2] = sensores.cota[6];
      mapaResultado[sensores.posF - 2][sensores.posC - 1] = sensores.superficie[7];
      mapaCotas[sensores.posF - 2][sensores.posC - 1] = sensores.cota[7];
      mapaResultado[sensores.posF - 2][sensores.posC] = sensores.superficie[8];
      mapaCotas[sensores.posF - 2][sensores.posC] = sensores.cota[8];
      mapaResultado[sensores.posF][sensores.posC - 3] = sensores.superficie[9];
      mapaCotas[sensores.posF][sensores.posC - 3] = sensores.cota[9];
      mapaResultado[sensores.posF - 1][sensores.posC - 3] = sensores.superficie[10];
      mapaCotas[sensores.posF - 1][sensores.posC - 3] = sensores.cota[10];
      mapaResultado[sensores.posF - 2][sensores.posC - 3] = sensores.superficie[11];
      mapaCotas[sensores.posF - 2][sensores.posC - 3] = sensores.cota[11];
      mapaResultado[sensores.posF - 3][sensores.posC - 3] = sensores.superficie[12];
      mapaCotas[sensores.posF - 3][sensores.posC - 3] = sensores.cota[12];
      mapaResultado[sensores.posF - 3][sensores.posC - 2] = sensores.superficie[13];
      mapaCotas[sensores.posF - 3][sensores.posC - 2] = sensores.cota[13];
      mapaResultado[sensores.posF - 3][sensores.posC - 1] = sensores.superficie[14];
      mapaCotas[sensores.posF - 3][sensores.posC - 1] = sensores.cota[14];
      mapaResultado[sensores.posF - 3][sensores.posC] = sensores.superficie[15];
      mapaCotas[sensores.posF - 3][sensores.posC] = sensores.cota[15];
      break;
  }
}



/**
 * @brief Determina si una casilla es transitable para el técnico.
 * En esta práctica, si el técnico tiene zapatillas, el bosque ('B') es transitable.
 * @param f Fila de la casilla.
 * @param c Columna de la casilla.
 * @param tieneZapatillas Indica si el agente posee las zapatillas.
 * @return true si la casilla es transitable.
 */
bool ComportamientoTecnico::EsCasillaTransitableLevel0(int f, int c, bool tieneZapatillas) {
  if (f < 0 || f >= mapaResultado.size() || c < 0 || c >= mapaResultado[0].size()) return false;
  return es_camino(mapaResultado[f][c]);  // Solo 'C', 'S', 'D', 'U' son transitables en Nivel 0
}

/**
 * @brief Comprueba si la casilla de delante es accesible por diferencia de altura.
 * Para el técnico: desnivel máximo siempre 1.
 * @param actual Estado actual del agente (fila, columna, orientacion).
 * @return true si el desnivel con la casilla de delante es admisible.
 */
bool ComportamientoTecnico::EsAccesiblePorAltura(const ubicacion &actual) {
  ubicacion del = Delante(actual);
  if (del.f < 0 || del.f >= mapaCotas.size() || del.c < 0 || del.c >= mapaCotas[0].size()) return false;
  int desnivel = abs(mapaCotas[del.f][del.c] - mapaCotas[actual.f][actual.c]);
  if (desnivel > 1) return false;
  return true;
}

/**
 * @brief Devuelve la posición (fila, columna) de la casilla que hay delante del agente.
 * Calcula la casilla frontal según la orientación actual (8 direcciones).
 * @param actual Estado actual del agente (fila, columna, orientacion).
 * @return Estado con la fila y columna de la casilla de enfrente.
 */
ubicacion ComportamientoTecnico::Delante(const ubicacion &actual) const {
  ubicacion delante = actual;
  switch (actual.brujula) {
    case 0: delante.f--; break;                        // norte
    case 1: delante.f--; delante.c++; break;     // noreste
    case 2: delante.c++; break;                     // este
    case 3: delante.f++; delante.c++; break;     // sureste
    case 4: delante.f++; break;                        // sur
    case 5: delante.f++; delante.c--; break;     // suroeste
    case 6: delante.c--; break;                     // oeste
    case 7: delante.f--; delante.c--; break;     // noroeste
  }
  return delante;
}


/**
 * @brief Imprime por consola la secuencia de acciones de un plan.
 *
 * @param plan  Lista de acciones del plan.
 */
void ComportamientoTecnico::PintaPlan(const list<Action> &plan)
{
  auto it = plan.begin();
  while (it != plan.end())
  {
    if (*it == WALK)
    {
      cout << "W ";
    }
    else if (*it == JUMP)
    {
      cout << "J ";
    }
    else if (*it == TURN_SR)
    {
      cout << "r ";
    }
    else if (*it == TURN_SL)
    {
      cout << "l ";
    }
    else if (*it == COME)
    {
      cout << "C ";
    }
    else if (*it == IDLE)
    {
      cout << "I ";
    }
    else
    {
      cout << "-_ ";
    }
    it++;
  }
  cout << "( longitud " << plan.size() << ")" << endl;
}



/**
 * @brief Convierte un plan de acciones en una lista de casillas para
 *        su visualización en el mapa 2D.
 *
 * @param st    Estado de partida.
 * @param plan  Lista de acciones del plan.
 */
void ComportamientoTecnico::VisualizaPlan(const ubicacion &st,
                                            const list<Action> &plan)
{
   listaPlanCasillas.clear();
  ubicacion cst = st;

  listaPlanCasillas.push_back({cst.f, cst.c, WALK});
  auto it = plan.begin();
  while (it != plan.end())
  {

    switch (*it)
    {
    case JUMP:
      switch (cst.brujula)
      {
      case 0:
        cst.f--;
        break;
      case 1:
        cst.f--;
        cst.c++;
        break;
      case 2:
        cst.c++;
        break;
      case 3:
        cst.f++;
        cst.c++;
        break;
      case 4:
        cst.f++;
        break;
      case 5:
        cst.f++;
        cst.c--;
        break;
      case 6:
        cst.c--;
        break;
      case 7:
        cst.f--;
        cst.c--;
        break;
      }
      if (cst.f >= 0 && cst.f < mapaResultado.size() &&
          cst.c >= 0 && cst.c < mapaResultado[0].size())
        listaPlanCasillas.push_back({cst.f, cst.c, JUMP});
    case WALK:
      switch (cst.brujula)
      {
      case 0:
        cst.f--;
        break;
      case 1:
        cst.f--;
        cst.c++;
        break;
      case 2:
        cst.c++;
        break;
      case 3:
        cst.f++;
        cst.c++;
        break;
      case 4:
        cst.f++;
        break;
      case 5:
        cst.f++;
        cst.c--;
        break;
      case 6:
        cst.c--;
        break;
      case 7:
        cst.f--;
        cst.c--;
        break;
      }
      if (cst.f >= 0 && cst.f < mapaResultado.size() &&
          cst.c >= 0 && cst.c < mapaResultado[0].size())
        listaPlanCasillas.push_back({cst.f, cst.c, WALK});
      break;
    case TURN_SR:
      cst.brujula = (Orientacion) (( (int) cst.brujula + 1) % 8);
      break;
    case TURN_SL:
      cst.brujula = (Orientacion) (( (int) cst.brujula + 7) % 8);
      break;
    }
    it++;
  }
}



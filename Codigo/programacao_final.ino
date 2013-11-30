#include <Servo.h>                                                  // Classe Servo
#include <LiquidCrystal.h>                                          // Classe LCD

int estado,velocidade;                                             // variaveis de controle
int pot=A0;                                                        // define potenciometro elo 1
int pot2=A1;                                                       // define potenciometro elo 2

int i=0,m=0,n=0,x=0,z=0,q=0,w=0,y=0,f=0,r=0,t=0,p=0; s=120               // variaveis de controle
Servo servo,servo2,servo3,servo4;                                         // Instancia da classe Servo
int Leitura[10],Leitura2[10],Leitura3[10];                         // variaveis pertencentes ao filtro
int Total=0,Total2=0,Total3=0;                                     // variaveis pertencentes ao filtro
int Media=0,Media2=0,Media3=0;                                     // variaveis de media para filtro (remocao de ruidos na leitura dos potenciometros)
int time=0;                                                        // tempo (velocidade) de atualizacao dos dados, ou seja, velocidade de movimentacao dos servos
int btn_cremalheira_cima=18;                                       // botao para subir cremalheira
int btn_cremalheira_baixo=19;                                      // botao para descer cremalheira
int val_crem_cima=0,val_crem_baixo=0,ct1_garra=0,ct2_garra=0, val_controle=0;      // declara variaveis de contagem
int botao_garra1=8;
int botao_garra2=A2;
int botao_controle=A3;                                             // saida da garra porta A3
//Linhas abaixo comentadas pois no lugar do motor dc foi inserido um servomotor (servo 4)
//int saida_garra1=A3;
//int saida_garra2=10;
String val_garra="N";

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);                            // Instancia da Classe LiquidCrystal definindo pinos correspondentes do LCD

void setup() {                

    lcd.begin(16, 2);                                            // Parametro quantidade de colunas x linhas LCD
    servo.attach(7);                                             // Define o servo elo 1
    servo2.attach(9);                                            // Define servo elo 2
    servo3.attach(13);                                           // Define servo cremalheira
    servo4.attach(10);                                           // Define servo garra
    
    Serial.begin(9600);                                          // Inicializa a Porta Serial com Baud Rate 9600 (standart)
    
    pinMode(btn_cremalheira_cima,INPUT);                         // utiliza pino analogico como digital
    pinMode(btn_cremalheira_baixo,INPUT);                        // utiliza pino analogico como digital
   // pinMode(saida_garra1,OUTPUT);                                // define saida motor 1 garra (COMENTADO DEVIVO A TROCA POR SERVO)
   // pinMode(saida_garra2,OUTPUT);                                // define saida motor 2 garra (COMENTADO DEVIVO A TROCA POR SERVO)
//    pinMode(botao_sel,INPUT);
    pinMode(botao_garra1,INPUT);                                 // define entrada botao ABRIR GARRA
    pinMode(botao_garra2,INPUT);                                 // define entrada botao FECHAR GARRA
    
    digitalWrite(btn_cremalheira_cima,HIGH);                     //ativa pullup interno
    digitalWrite(btn_cremalheira_baixo,HIGH);                    //ativa pullup interno 
    digitalWrite(botao_garra1, HIGH);                            // ativa pull-up interno
    digitalWrite(botao_garra2, HIGH);                            // ativa pull-up interno

    //Limpa da memoria vetores do filtro de rumor
    for(f=0; f< 10; f++) {
     Leitura[f] = 0;

    }
     f=0;
         for(r=0; r< 10; r++) {
     Leitura2[r] = 0;

    }
     r=0;
             for(p=0; p< 10; p++) {
     Leitura3[p] = 0;

    }
     p=0;
}

void loop() {                                                                     // Inicio da linha de execucao do programa
   filtro_servo1();                                                              // Executa filtro 1
   filtro_servo2();                                                              // Executa filtro 2

   val_crem_cima = digitalRead(btn_cremalheira_cima);                            // Leitura botao de subir cremalheira
   val_crem_baixo = digitalRead(btn_cremalheira_baixo);                          // Leitura botao de descer cremalheira
  
   if  ((val_crem_cima==LOW)&&(val_crem_baixo==HIGH)) {                         //logica inversa, pois pullip interno esta ativado
        t=t+1;
       } 
   if  ((val_crem_baixo==LOW)&&(val_crem_cima==HIGH)&&(t>0)) {
        t=t-1;
       }
     
   ct1_garra = digitalRead(botao_garra1);                                       // Leitura botao 1 da garra
   ct2_garra = digitalRead(botao_garra2);                                       // Leitura botao 2 da garra
       
   if ((ct1_garra==LOW)&&(ct2_garra==HIGH)){                                    // Aciona motor 1 da garra e caso botao 2 estiver acionado ignora
        val_garra="A";                                                          // Indicacao estado garra ABERTA
        digitalWrite(saida_garra1,HIGH);
      }else{
        digitalWrite(saida_garra1,LOW);
      }
     
   if ((ct2_garra==HIGH)&&(ct1_garra==LOW)){                                    // Aciona motor 2 da garra e caso botao 1 estiver acionado ignora
        val_garra="F";                                                          // Indicacao estado garra FECHADA
        digitalWrite(saida_garra2,HIGH);
      }else{
        digitalWrite(saida_garra2,LOW); 
      }
      
   if (((ct2_garra==HIGH)&&(ct1_garra==HIGH))){                                 // Caso os dois botoes da garra forem acionados NADA faz
        val_garra="N";
      } 
      
     if ((ct2_garra==LOW)&&(ct1_garra==LOW)){                                   // Caso os dois botoes da garra NAO forem acionados sao desligados os motores
        val_garra="N";
        digitalWrite(saida_garra1,LOW); 
        digitalWrite(saida_garra2,LOW); 
      }   
      
   i= servo.read();                                                            //Leitura posicao do Servo elo 1
        
        if(i<x){                                                               // Caso a posicao do servo seja menor do que a indicada no potenciometro do elo 1 
        i=i+1;                                                                 // A variavel e' incrementada com resolucao igual a 1
        servo.write(i);                                                        // A nova posicao è enviada ao servo elo 1 gradualmente (a cada loop è incrementado um valor igual a resolucao)
        delay(time);                                                           // velocidade de transicao das posicoes antiga e atual
        }
        
        exibedisplay();
         
   q= servo2.read();                                                           //Leitura posicao do Servo elo 2
        
        if(q<y){      
        q=q+1; 
        servo2.write(q);
        delay(time);  
        }
        
   m= servo3.read();                                                          //Leitura posicao do Servo cremalheira
        
        if(m<t){      
        m=m+1; 
        servo3.write(m);
        delay(time);
        }  

   z= servo.read();                                                          //Leitura posicao do Servo elo 1, a fim de armazenar a nova posicao em outra variavel


      
        if(z>x){                                                             // Caso a posicao do servo seja maior do que a indicada no potenciometro do elo 1 
        
        z=z-1;                                                               // A variavel e' decrementada com resolucao igual a 1
        servo.write(z);                                                      // A nova posicao è enviada ao servo elo 1 gradualmente (a cada loop è incrementado um valor igual a resolucao)
        delay(time);                                                         // velocidade de transicao das posicoes antiga e atual
        }

   w= servo2.read();

       
        if(w>y){       
        
        w=w-1;     
        servo2.write(w);
        delay(time);

        }
        
   n= servo3.read();

        if(n>t){       
        
        n=n-1;     
        servo3.write(n);
        delay(time);

        }
        
        if ((i!=x)||(y!=q)||(m!=t)){            // Atualiza linha 1 do LCD
           LcdClearLine(1);                     // Executa funcao de limpeza da linha 1
           exibedisplay();                      // Exibe os dados atuais
        }
        
} // fim do loop
    
void LcdClearLine(int r) {                      // Declara Funcao com retorno vazio
      lcd.setCursor(0,r);                       // seta o cursor do LCD na posicao 'r'
      for (int ii = 0; ii < 16; ii = ii + 1)    // faz a varredura das colunas do inicio ao fim do LCD
      {
        lcd.print(" ");                         // Limpa a coluna
      }
}

void exibedisplay(){                            // Funcao responsavel por exibir dados no display de LCD 

            lcd.setCursor(1, 0);                // Seta posicao do cursor no LCD
     lcd.print("E1 ");                          // Exibe legenda representando ELO 1
            lcd.setCursor(5, 0);
     lcd.print("E2 ");                          // Exibe legenda representando ELO 2
            lcd.setCursor(9, 0);
     lcd.print("E3 ");                          // Exibe legenda representando ELO 3 (cremalheira)
            lcd.setCursor(13, 0);
     lcd.print("G1 ");                          // Exibe legenda representando GARRA
            lcd.setCursor(1, 1);
     lcd.print(x);                              // Exibe posicao do ELO 1
            lcd.setCursor(5, 1);
     lcd.print(y);                              // Exibe posicao do ELO 2
            lcd.setCursor(9, 1);
     lcd.print(t);                              // Exibe posicao do ELO 3 (cremalheira)    
            lcd.setCursor(13, 1);
     lcd.print(val_garra);                      // Exibe posicao da GARRA

}
 
void exibe(){                                  // Exibe os valores na porta Serial do computador (somente necessaria a nivel de teste e calibragem)
     Serial.print("x= ");
     Serial.print(x);
     Serial.print("\t");
     Serial.print("i= ");
     Serial.print(i);
     Serial.print("\t");
     Serial.print("z= ");
     Serial.print(z);
     Serial.print("\t");
     Serial.print("y= ");
     Serial.print(y);
     Serial.print("\t");
     Serial.print("q= ");
     Serial.print(q);
     Serial.print("\t");
     Serial.print("w= ");
     Serial.print(w);
     Serial.print("\n");     
}

void filtro_servo1(){                          //Filtro Potenciomentro
    Total= Total - Leitura[f];                // Subtrai do total a leitura anterior
    Leitura[f] =analogRead(pot);              // Armazena valor analogico do potenciometro na posicao 'f' do vetor
    Total = Total + Leitura[f];               // Soma valor atual ao montante Total
    f = f + 1;                                // Incrementa contador responsavel pelo avanco no vetor
    if (f >= 10){                             
      f = 0;                                  // Zero vetor
      Media = Total / 10;                     // Calcula a media
      x= map(Media,0,1023,0,179);             // Mapeia o intervalo de valores de 0 a 180 (graus)
    }
}

void filtro_servo2(){
    Total2=Total2 - Leitura2[r];
    Leitura2[r] =analogRead(pot2);
    Total2 = Total2 + Leitura2[r];
    r = r + 1;
    if (r >= 10){
      r = 0;
      Media2 = Total2 / 10;
      y= map(Media2,0,1023,0,179);
      }
}
    
      
     


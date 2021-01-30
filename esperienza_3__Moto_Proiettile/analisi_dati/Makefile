all:	analisi_dati

analisi_dati: analisi_dati_201218_v0.cpp
		g++ -o analisi_dati analisi_dati_201218_v0.cpp $$(root-config --cflags --glibs)

run:	./analisi_dati

clean:	rm -f *~ analisi_dati

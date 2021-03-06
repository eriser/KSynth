

#include <QApplication>
#include <QMessageBox>

#include <KSynth/midi/rt/RtMidiWrapper.h>
#include <KSynth//midi/rt/RtMidiInDevice.h>
#include <KSynth/midi/MidiParser.h>
#include <KSynth/Sequencer.h>
#include <KSynth/Generator.h>
#include <KSynth/fx/Phaser.h>

#include "SynthWin.h"
#include "SequencerWidget.h"
#include "RackWidget.h"
#include "MainWin.h"
#include "ControlPanelWidget.h"
#include "MidiBindingDialog.h"

#include "controller/Controller.h"

#include "rack/Rack.h"
#include "rack/RackFactory.h"
#include "rack/Synth1.h"
#include "rack/Sequencer1.h"
#include "rack/Reverb1.h"
#include "rack/Mixer1.h"
#include "rack/DrumComputer1.h"
#include "rack/MasterTarget1.h"
#include "rack/SamplePad1.h"
#include "rack/Reverb1.h"
#include "rack/Phaser1.h"

#include <KSynth/fx/StereoExpander.h>

#include "KSynth/fx/Vocoder.h"

#include <QFile>
#include <QTextStream>

#include "controller/tasks/Tasks.h"

#include <KLib/memory/FixedPool.h>

/** display error message */
void showError(std::string err) {
	err += "\n\nthe program will now terminate!";
	int ret = QMessageBox::critical(nullptr, QString("an exception occurred..."), QString(err.c_str()), QMessageBox::Ok, QMessageBox::Ok);
	(void) ret;
	exit(-1);
}


/** load style-sheet from skins folder */
void loadStyleSheet() {

	QFile data("skin/stylesheet/style.css");
	QString style;

	// try ot open file
	if( !data.open(QFile::ReadOnly) ) {return;}

	QTextStream styleIn(&data);
	style = styleIn.readAll();
	data.close();

	qApp->setStyleSheet(style);

}


/**
 * @brief Application wrapper to catch exceptions
 */
class MyApplication : public QApplication {


public:

	/** ctor */
	MyApplication(int& argc, char** argv) : QApplication(argc, argv) {;}

	/** dtor */
	virtual ~MyApplication() {;}

	/** catch exceptions */
	virtual bool notify(QObject* receiver, QEvent* event) {
//		try {
			return QApplication::notify(receiver, event);

//		} catch(std::exception& e) {
//			showError(e.what());

//		} catch (...) {
//			showError("an unknown error occured.");

//		}

		return false;
	}

};

#include <SampleChooserDialog.h>
#include "misc/Skin.h"


/** initialize workspace using a task */
class InitTask : public Task {

	Context& ctx;
	MainWin& mw;

public:

	InitTask(Context& ctx) :
		Task("initializing", true), ctx(ctx), mw(*ctx.getMainWindow()) {;}

	void exec() override {

		// create rack panel
		setProgress("creating rack", 0.2f);
		RackWidget* rw = new RackWidget(ctx);

		// create Rack-Model
		ctx.rack = new Rack(ctx, *rw);

		// create sequencer panel
		setProgress("creating sequencer", 0.4f);
		SequencerWidget* sw = new SequencerWidget(ctx);

		// create control panel
		setProgress("creating control panel", 0.6f);
		ControlPanelWidget* cpw = new ControlPanelWidget(ctx);

		// add master target
		setProgress("creating master target", 0.8f);
		MasterTarget1* dst = new MasterTarget1(ctx);
		ctx.setMasterTarget(dst);
		ctx.getRack()->add(dst);


		// add dock widgets
		setProgress("docking widgets", 0.9f);
		mw.addDockWidget(Qt::LeftDockWidgetArea, rw);
		mw.addDockWidget(Qt::TopDockWidgetArea, sw);
		mw.addDockWidget(Qt::BottomDockWidgetArea, cpw);

		// TESTING
		//K::File f("/data/midi/superMario/smwintro.mid");
		//K::File f("/data/midi/duke1.mid");
		//K::File f("/apps/workspaces/KSynth/_build/mainmenu.mid");
		K::File f("/apps/workspaces/KSynth/_build/ylvis-the_fox_what_does_the_fox_say.mid");

		if (f.isFile()) {

			setProgress("loading midi", 0.1f);

			if (1 == 0) {

				MidiFile midi;
				MidiParser(f, midi);
				ctx.getSequencer()->import(midi,0);

			} else {

				SequencerTrack* st = new SequencerTrack();
				MidiEvent me1, me2;
				me1.setType(MidiEventType::NOTE_ON);
				me1.setDelay(10);
				me1.setData1(69);
				me1.setData2(100);
				me2.setType(MidiEventType::NOTE_OFF);
				me2.setDelay(10000);
				me2.setData1(69);
				me2.setData2(0);


				st->getEvents()->add(me1);st->getEvents()->add(me2);
				ctx.getSequencer()->addTrack(new SequencerTrack());
				ctx.getSequencer()->addTrack(st);
				ctx.getSequencer()->addTrack(new SequencerTrack());
				ctx.getSequencer()->addTrack(new SequencerTrack());
				ctx.getSequencer()->addTrack(new SequencerTrack());
				ctx.getSequencer()->addTrack(new SequencerTrack());
				ctx.getSequencer()->addTrack(new SequencerTrack());

			}




			Vocoder* voc = new Vocoder();


			Reverb1* rev = new Reverb1(ctx);								ctx.getRack()->add(rev);

			setProgress("loading mixer", 0.2f);
			Mixer1* mixer = new Mixer1(ctx); mixer->setUserName("mixer");	ctx.getRack()->add(mixer);

			Phaser1* ph = new Phaser1(ctx); ctx.getRack()->add(ph);

			//StereoExpander* se = new StereoExpander();

			setProgress("loading synth", 0.3f);
			Synth1* s1 = new Synth1(ctx); s1->setUserName("synth 1");		ctx.getRack()->add(s1);
			setProgress("loading synth", 0.4f);
			Synth1* s2 = new Synth1(ctx); s2->setUserName("synth 2");		ctx.getRack()->add(s2);
			setProgress("loading synth", 0.5f);
			Synth1* s3 = new Synth1(ctx); s3->setUserName("synth 3");		ctx.getRack()->add(s3);
			setProgress("loading synth", 0.6f);
			Synth1* s4 = new Synth1(ctx); s4->setUserName("synth 4");		ctx.getRack()->add(s4);
			setProgress("loading synth", 0.7f);
			Synth1* s5 = new Synth1(ctx); s5->setUserName("synth 5");		ctx.getRack()->add(s5);
			setProgress("loading synth", 0.8f);
			Synth1* s6 = new Synth1(ctx); s6->setUserName("synth 6");		ctx.getRack()->add(s6);
			setProgress("loading drums", 0.9f);
			DrumComputer1* drums = new DrumComputer1(ctx); drums->setUserName("drums");	ctx.getRack()->add(drums);
			SamplePad1 * sp1 = new SamplePad1(ctx); sp1->setUserName("sample pad 1"); ctx.getRack()->add(sp1);



			ctx.getGenerator()->getBinder().addBinding(0, s1, 0, mixer);
			ctx.getGenerator()->getBinder().addBinding(1, s1, 1, mixer);
			ctx.getGenerator()->getBinder().addBinding(0, s2, 2, mixer);
			ctx.getGenerator()->getBinder().addBinding(1, s2, 3, mixer);
			ctx.getGenerator()->getBinder().addBinding(0, s3, 4, mixer);
			ctx.getGenerator()->getBinder().addBinding(1, s3, 5, mixer);
			ctx.getGenerator()->getBinder().addBinding(0, s4, 6, mixer);
			ctx.getGenerator()->getBinder().addBinding(1, s4, 7, mixer);
			ctx.getGenerator()->getBinder().addBinding(0, s5, 8, mixer);
			ctx.getGenerator()->getBinder().addBinding(1, s5, 9, mixer);
			ctx.getGenerator()->getBinder().addBinding(0, s6, 10, mixer);
			ctx.getGenerator()->getBinder().addBinding(1, s6, 11, mixer);
			ctx.getGenerator()->getBinder().addBinding(0, sp1, 14, mixer);
			ctx.getGenerator()->getBinder().addBinding(1, sp1, 15, mixer);
			//ctx.getGenerator()->getBinder().addBinding(0, drums, 16, mixer);
			//ctx.getGenerator()->getBinder().addBinding(1, drums, 17, mixer);

			ctx.getGenerator()->getBinder().addBinding(0, mixer, 0, ph);
			ctx.getGenerator()->getBinder().addBinding(1, mixer, 1, ph);

			/*
			drums->loadSample("/data/test.wav", 0);
			ctx.getGenerator()->getBinder().addBinding(0, drums, 0, voc);
			ctx.getGenerator()->getBinder().addBinding(1, drums, 1, voc);
			ctx.getGenerator()->getBinder().addBinding(0, rev, 2, voc);
			ctx.getGenerator()->getBinder().addBinding(1, rev, 3, voc);
			ctx.getGenerator()->getBinder().addBinding(0, voc, 0, ctx.getMasterTarget());
			ctx.getGenerator()->getBinder().addBinding(1, voc, 1, ctx.getMasterTarget());
			*/

			ctx.getGenerator()->getBinder().addBinding(0, ph, 0, rev);
			ctx.getGenerator()->getBinder().addBinding(1, ph, 1, rev);

			//ctx.getGenerator()->getBinder().addBinding(0, rev, 0, se);
			//ctx.getGenerator()->getBinder().addBinding(1, rev, 1, se);

			ctx.getGenerator()->getBinder().addBinding(0, rev, 0, ctx.getMasterTarget());
			ctx.getGenerator()->getBinder().addBinding(1, rev, 1, ctx.getMasterTarget());

			ctx.getSequencer()->setBeatsPerMinute(85);

			setProgress("binding", 0.95f);
			ctx.getSequencer()->bind(1, s1);
			ctx.getSequencer()->bind(2, s2);
			ctx.getSequencer()->bind(3, s3);
			ctx.getSequencer()->bind(4, s4);
			ctx.getSequencer()->bind(5, s5);
			ctx.getSequencer()->bind(6, s6);

			mixer->setParameter( (int) SimpleMixerParams::MASTER_VOLUME, 0.15f);

		}

		// load settings
		setProgress("loading settings", 0.95f);
		ctx.getController()->loadSettings();

	}

};


/** start everything */
int main(int argc, char *argv[]) {


	MyApplication app(argc, argv);

	Context ctx(app);

	// create task sheduler
	ctx.tasks = new Tasks(ctx.getMainWindow());

	// check skin installation
	Skin::checkPresent(ctx, *ctx.getMainWindow());

	// load stylesheet from skin
	loadStyleSheet();

	// initialize the rack
	ctx.getTasks()->addTaskForeground( new InitTask(ctx) );

	// done
	ctx.seq->setBeatsPerMinute(90);
	ctx.getRack()->setRefreshing(true);

	return app.exec();

}

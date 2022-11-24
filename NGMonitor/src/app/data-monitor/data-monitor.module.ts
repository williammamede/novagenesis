import { NgModule } from '@angular/core';
import { DataMonitorComponent } from './data-monitor.component';
import { DataMonitorService } from './data-monitor.service';

import { MatIconModule } from '@angular/material/icon';
import { MatListModule } from '@angular/material/list';
import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MatProgressBarModule } from '@angular/material/progress-bar';
import { MatTabsModule } from '@angular/material/tabs';
import { MatDialogModule } from '@angular/material/dialog';
import { MatTableModule } from '@angular/material/table';
import { MatButtonModule } from '@angular/material/button';
import { DataViewerDialogComponent } from './data-viewer-dialog/data-viewer-dialog.component';

@NgModule({
    declarations: [
        DataMonitorComponent,
        DataViewerDialogComponent
    ],
    imports: [
        MatIconModule,
        MatListModule,
        BrowserModule,
        BrowserAnimationsModule,
        FormsModule,
        ReactiveFormsModule,
        MatProgressBarModule,
        MatTabsModule,
        MatDialogModule,
        MatTableModule,
        MatButtonModule
    ],
    exports: [
        DataMonitorComponent
    ],
    providers: [DataMonitorService],
    bootstrap: [DataMonitorComponent]
})

export class DataMonitorModule { }
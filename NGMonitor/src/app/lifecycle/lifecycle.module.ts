import { NgModule } from '@angular/core';
import { LifecycleComponent } from './lifecycle.component';

import { MatIconModule } from '@angular/material/icon';
import { MatListModule } from '@angular/material/list';
import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MatProgressBarModule } from '@angular/material/progress-bar';
import { MatProgressSpinnerModule } from '@angular/material/progress-spinner';
import { MatTabsModule } from '@angular/material/tabs';
import { MatDialogModule } from '@angular/material/dialog';
import { MatTableModule } from '@angular/material/table';
import { MatButtonModule } from '@angular/material/button';
import { LifecycleService } from './lifecycle.service';

@NgModule({
    declarations: [
        LifecycleComponent
    ],
    imports: [
        MatIconModule,
        MatListModule,
        BrowserModule,
        BrowserAnimationsModule,
        FormsModule,
        ReactiveFormsModule,
        MatProgressBarModule,
        MatProgressSpinnerModule,
        MatTabsModule,
        MatDialogModule,
        MatTableModule,
        MatButtonModule
    ],
    exports: [],
    providers: [LifecycleService],
    bootstrap: [LifecycleComponent]
})

export class LifecycleModule { }
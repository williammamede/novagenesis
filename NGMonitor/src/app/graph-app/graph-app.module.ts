import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { GraphAppComponent } from './graph-app.component';
import { HttpClientModule } from '@angular/common/http';
import { GraphAppService } from './graph-app.service';
import { NgxGraphModule } from '@swimlane/ngx-graph';
import { MatCardModule } from '@angular/material/card';
import { MatProgressBarModule } from '@angular/material/progress-bar';

@NgModule({
    declarations: [
        GraphAppComponent,
    ],
    imports: [
        BrowserModule,
        HttpClientModule,
        NgxGraphModule,
        MatCardModule,
        MatProgressBarModule
    ],
    exports: [
        GraphAppComponent
    ],
    providers: [GraphAppService],
    bootstrap: [GraphAppComponent]
})
export class GraphAppModule { }